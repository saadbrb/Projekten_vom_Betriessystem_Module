FROM debian:latest
MAINTAINER Ly-Saad
WORKDIR /home/saad/Schreibtisch/Prak 4 BS/Example

# Install g++
RUN apt-get update && apt-get install -y g++

COPY main.cpp .

# Compile the program
RUN g++ -o mydocker main.cpp

CMD ["./mydocker"]

