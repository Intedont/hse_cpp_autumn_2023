all: install test

install: cjson.c
	sudo python3 setup.py install

test:
	sudo python3 test.py

clean:
	sudo rm -r build dist cjson.egg-info