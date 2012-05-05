#!/bin/bash
`./arma`
$(`qemu -s -S -m 256 -cdrom BrunOS.iso &`)
