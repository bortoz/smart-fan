init:
	west init .
	west update

build:
	west build app

flash:
	west flash

monitor:
	west espressif monitor

.PHONY: init build flash monitor
