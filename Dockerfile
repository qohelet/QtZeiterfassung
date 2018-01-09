FROM ubuntu:artful

RUN apt update \
 && apt install git-core g++ make qt5-default qtmultimedia5-dev qttools5-dev qttools5-dev-tools -y \
 && git clone --recursive https://github.com/0xFEEDC0DE64/QtZeiterfassung.git /tmp/QtZeiterfassung \
 && mkdir -p /tmp/build_QtZeiterfassung \
 && qmake /tmp/QtZeiterfassung -o /tmp/build_QtZeiterfassung/Makefile -config release \
 && make -C /tmp/build_QtZeiterfassung -j8 \
 && make -C /tmp/build_QtZeiterfassung install \
 && mv /tmp/build_QtZeiterfassung/bin /root

CMD /root/bin/zeiterfassung.sh
