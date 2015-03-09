Spawn example
=====

export SYSTEMC_HOME=$(ls -d /usr/local/systemc-*)

g++ -I. -I$SYSTEMC_HOME/include \
    -L. -L$SYSTEMC_HOME/lib-linux64 \
    -Wl,-rpath=$SYSTEMC_HOME/lib-linux64 \
    -o patito modulo_pato.cc \
    -lsystemc -lm
