FROM python:3.9-slim

RUN pip install platformio

COPY build.sh /build.sh

ENTRYPOINT ["/build.sh"]