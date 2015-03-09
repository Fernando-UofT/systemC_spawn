Spawn example
=====

export SYSTEMC_HOME=/usr/local/systemc-2.3.0

g++ -I. -I$SYSTEMC_HOME/include \
    -L. -L$SYSTEMC_HOME/lib-linux64 \
    -Wl,-rpath=$SYSTEMC_HOME/lib-linux64 \
    -o patito modulo_pato.cpp \
    -lsystemc -lm
