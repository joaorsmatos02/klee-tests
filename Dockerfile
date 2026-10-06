# KLEE built with the fork that implements the suite's file-system API. The
# image holds KLEE only: the suite is mounted when it runs.
#
# The fork's source is passed as the "fork" build context, either a local
# checkout or the fork on GitHub:
#
#   docker build -t klee-tests --build-context fork=$HOME/klee .
#   docker build -t klee-tests --build-context fork=https://github.com/dino-fan777/klee.git#shared-testsuite .
#
# Then run the suite in it:
#
#   docker run -it --rm -v "$PWD":/home/klee/klee-tests klee-tests

FROM klee/klee:3.1

# jq, for the JSON results. The base image's kitware apt source has an expired
# key, so "apt-get update" reports an error but still updates Ubuntu's.
USER root
RUN (apt-get update -qq || true) \
 && apt-get install -y --no-install-recommends jq \
 && rm -rf /var/lib/apt/lists/*
USER klee

# Copy the files the fork changes over KLEE 3.1's source, and rebuild KLEE.
# The base image has KLEE already built, so only these files recompile.
COPY --from=fork --chown=klee:klee \
     /runtime/POSIX/fd.c /runtime/POSIX/fd.h /runtime/POSIX/fd_init.c \
     /runtime/POSIX/file_api.c /runtime/POSIX/CMakeLists.txt \
     /tmp/klee_src/runtime/POSIX/
COPY --from=fork --chown=klee:klee \
     /include/klee/file_api.h /include/klee/klee.h \
     /tmp/klee_src/include/klee/
COPY --from=fork --chown=klee:klee \
     /lib/Core/SpecialFunctionHandler.cpp /lib/Core/SpecialFunctionHandler.h \
     /tmp/klee_src/lib/Core/
RUN rm -rf /tmp/klee_build130stp_z3/runtime/lib/libkleeRuntimePOSIX64_* \
           /tmp/klee_build130stp_z3/runtime/POSIX/CMakeFiles \
 && make -C /tmp/klee_build130stp_z3 -j"$(nproc)"

ENV PATH=/home/klee/klee_build/bin:$PATH
WORKDIR /home/klee/klee-tests
CMD ["/bin/bash", "-lc", "cat workflow.txt; exec bash"]
