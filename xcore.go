package main

import (
	"github.com/jurgen-kluft/xcode"
	"github.com/jurgen-kluft/xcore/package"
)

func main() {
	xcode.Init()
	xcode.Generate(xcore.GetPackage())
}
