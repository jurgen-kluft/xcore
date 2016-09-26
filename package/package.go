package xcore

import (
	"github.com/jurgen-kluft/xcode/denv"
	"github.com/jurgen-kluft/xbase/package"
	"github.com/jurgen-kluft/xunittest/package"
)

// GetPackage returns the package object of 'xcore'
func GetPackage() *denv.Package {
	// Dependencies
	xbasepkg := xbase.GetPackage()
	unittestpkg := xunittest.GetPackage()

	// The main (xcore) package
	mainpkg := denv.NewPackage("xcore")
	mainpkg.AddPackage(xbasepkg)
	mainpkg.AddPackage(unittestpkg)

	// 'xcore' library
	mainlib := denv.SetupDefaultCppLibProject("xcore", "github.com\\jurgen-kluft\\xcore")
	mainlib.Dependencies = append(mainlib.Dependencies, unittestpkg.GetMainLib())

	// 'xcore' unittest project
	maintest := denv.SetupDefaultCppTestProject("xcore_test", "github.com\\jurgen-kluft\\xcore")

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
