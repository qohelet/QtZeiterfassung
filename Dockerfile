FROM ubuntu:artful

RUN apt update \
 && apt install git-core g++ make qt5-default qtmultimedia5-dev qttools5-dev qttools5-dev-tools -y \
 && rm /var/lib/apt/lists/* -Rf

ADD . /tmp/QtZeiterfassung

RUN mkdir -p /tmp/build_QtZeiterfassung \
 && qmake /tmp/QtZeiterfassung -o /tmp/build_QtZeiterfassung/Makefile -config release \
 && make -C /tmp/build_QtZeiterfassung -j8 \
 && make -C /tmp/build_QtZeiterfassung install



FROM ubuntu:artful

RUN apt update \
 && apt install libssl1.0.0 ca-certificates libqt5core5a libqt5gui5 libqt5network5 libqt5multimedia5 -y \
 && rm /var/lib/apt/lists/* -Rf

COPY --from=0 /tmp/build_QtZeiterfassung/bin /root/bin

CMD /root/bin/zeiterfassung.sh
