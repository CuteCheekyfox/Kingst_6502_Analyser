TARGET  := libWDC6502.so

LINK := -L "lib" -lAnalyzer

CC       := g++
HFILE    := inc/*.h
SRC      := src/*.cpp
INC      := -I inc/ -I src/
CXXFLAGS := -Wall -O2 -c
FPIC     := -fPIC
SHARE    := -shared -o
OBJ      := *.o

$(TARGET) : $(HFILe) $(SRC)
	$(CC) $(CXXFLAGS) $(FPIC) $(SRC) $(INC)
	$(CC) $(SHARE) $(TARGET) $(OBJ) $(LINK)
	
# Need to copy the compiled library to the Analyzer folder in the
# installed Kingst analyzer software

# cp libWDC6502.so ~/KingstVIS/Analyzer

