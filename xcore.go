package main

import (
	"github.com/jurgen-kluft/xcode"
	"github.com/jurgen-kluft/xcore/package"
)

func main() {
	xcode.Generate(xcore.GetPackage())
}
