FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y python3 python3-pip python3-simplejson python3-six git cmake build-essential wine64-development msitools ca-certificates winbind

# Install wine MSVC
RUN git clone https://github.com/mstorsjo/msvc-wine/ /opt/msvc
WORKDIR /opt/msvc
RUN PYTHONUNBUFFERED=1 ./vsdownload.py --accept-license --dest /opt/msvc && \
    ./install.sh /opt/msvc

WORKDIR /data

# Install smsdk cli
COPY requirements.txt /data/
RUN pip3 install -r requirements.txt
COPY smsdk.py /data/

ENTRYPOINT [ "python3", "smsdk.py" ]