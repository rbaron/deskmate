FROM alpine:3.12

RUN apk add --no-cache clang make

COPY lint.sh /lint.sh

ENTRYPOINT ["/lint.sh"]