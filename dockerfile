FROM ubuntu:22.04


RUN apt update && apt install -y git build-essential mkisofs
RUN git clone https://github.com/johannes-fetz/joengine.git

WORKDIR joengine

RUN ls

RUN mkdir samples samples/saturn-paeng
#COPY . samples/saturn-paeng

WORKDIR samples/saturn-paeng

# RUN ./compile.sh
CMD sleep 120