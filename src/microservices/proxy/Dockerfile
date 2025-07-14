# Dependency time ---------------------------------------
ARG BASE_DEPENDENCY_IMAGE=cpp_builder
FROM ${BASE_DEPENDENCY_IMAGE} AS kb_proxy_dependecies

ARG build_dir=/kb_proxy/build

ADD ./conanfile.txt /kb_proxy/deps/conanfile.txt
COPY --from=cpp_builder /deps/conanprofile.txt /kb_proxy/deps/conanprofile.txt

RUN /root/.local/bin/conan install \
  /kb_proxy/deps/conanfile.txt \
  --profile:all=/deps/conanprofile.txt \
  --output-folder=$build_dir/ --build=missing

# Compile time ---------------------------------------
FROM kb_proxy_dependecies AS build

ARG toolchaine=$build_dir/build/linux-x86_64-gcc-release/generators/conan_toolchain.cmake

COPY . /kb_proxy
WORKDIR $build_dir

RUN cmake .. --toolchain=$toolchaine \
  -DCMAKE_BUILD_TYPE=Release && \
  cmake --build . && \
  CTEST_OUTPUT_ON_FAILURE=TRUE cmake --build . --target test

# Run time ---------------------------------------
FROM debian:stable-slim

ARG PORT=8000
ARG MONOLITH_URL=http://monolith:8080
ARG MOVIES_SERVICE_URL=http://movies-service:8081
ARG EVENTS_SERVICE_URL=http://events-service:8082
ARG GRADUAL_MIGRATION=true
ARG MOVIES_MIGRATION_PERCENT=50

ENV PORT=$PORT
ENV MONOLITH_URL=$MONOLITH_URL
ENV MOVIES_SERVICE_URL=$MOVIES_SERVICE_URL
ENV EVENTS_SERVICE_URL=$EVENTS_SERVICE_URL
ENV GRADUAL_MIGRATION=$GRADUAL_MIGRATION
ENV MOVIES_MIGRATION_PERCENT=$MOVIES_MIGRATION_PERCENT

RUN groupadd -r yarch02 && useradd -r -g yarch02 yarch02
USER yarch02

WORKDIR /kb_proxy
COPY --from=build /kb_proxy/build/KbProxy .

RUN pwd && ls -alt .

EXPOSE 8000
ENTRYPOINT sh -c './KbProxy --port=$PORT --monolith-url="$MONOLITH_URL" \
  --gradual-migration="$GRADUAL_MIGRATION" --movies-migration-percent=$MOVIES_MIGRATION_PERCENT \
  --movies-service-url="$MOVIES_SERVICE_URL" --events-service-url="$EVENTS_SERVICE_URL"'
