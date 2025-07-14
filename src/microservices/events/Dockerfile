# Dependency time ---------------------------------------
ARG BASE_DEPENDENCY_IMAGE=cpp_builder
FROM ${BASE_DEPENDENCY_IMAGE}:latest AS kb_cpp_builder
FROM kb_cpp_builder AS kb_events_dependecies

ARG build_dir=/kb_events/build

ADD ./conanfile.txt /kb_events/deps/conanfile.txt
COPY --from=kb_cpp_builder /deps/conanprofile.txt /kb_events/deps/conanprofile.txt

RUN /root/.local/bin/conan install \
  /kb_events/deps/conanfile.txt \
  --profile:all=/deps/conanprofile.txt \
  --output-folder=$build_dir/ --build=missing

# Compile time ---------------------------------------
FROM kb_events_dependecies AS build

ARG toolchaine=$build_dir/build/linux-x86_64-gcc-release/generators/conan_toolchain.cmake

COPY . /kb_events
WORKDIR $build_dir

RUN cmake .. --toolchain=$toolchaine \
  -DCMAKE_BUILD_TYPE=Release && \
  cmake --build . && \
  CTEST_OUTPUT_ON_FAILURE=TRUE cmake --build . --target test

# Run time ---------------------------------------
FROM debian:stable-slim

ARG PORT=8082
ARG KAFKA_BROKERS=kafka:9092
ENV PORT=$PORT
ENV KAFKA_BROKERS=$KAFKA_BROKERS

RUN groupadd -r yarch02 && useradd -r -g yarch02 yarch02
USER yarch02

WORKDIR /kb_events
COPY --from=build /kb_events/build/KbEvents .

EXPOSE $PORT
ENTRYPOINT sh -c './KbEvents --port=$PORT --kafka-brokers="$KAFKA_BROKERS"'