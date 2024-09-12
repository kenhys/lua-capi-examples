TARGET = settable setfield

all: $(TARGET)

.c: $(TARGET)
	gcc $< -I`luarocks config --lua-incdir` -L`luarocks config --lua-libdir` -llua -ldl -lm -o $@
	./$@

clean:
	rm -f $(TARGET)
