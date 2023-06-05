FROM debian:bullseye-slim
ARG DEBIAN_FRONTEND=noninteractive

RUN \
	apt-get update && \
	apt-get install -y --no-install-recommends \
	build-essential \
	ca-certificates \
	cmake \
	curl \
	git \
	jq \
	libsdl2-dev \
	libopengl-dev

WORKDIR /DearQoi
