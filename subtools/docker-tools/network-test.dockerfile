FROM alpine:3.4
RUN apk update && \
apk add --no-cache tcpdump && \
apk add --no-cache iperf && \
apk add --no-cache ethtool
