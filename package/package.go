package xcore

import (
	"github.com/jurgen-kluft/xcode/denv"
	"github.com/jurgen-kluft/xunittest/package"
)

// GetPackage returns the package object of 'xcore'
func GetPackage() *denv.Package {
	// Dependencies
	unittestpkg := xunittest.GetPackage()

	// The main (xcore) package
	mainpkg := denv.NewPackage("xcore")
	mainpkg.AddPackage(unittestpkg)

	// 'xcore' library
	mainlib := denv.SetupDefaultCppLibProject("xcore", "github.com\\jurgen-kluft\\xcore")
	mainlib.Dependencies = append(mainlib.Dependencies, unittestpkg.GetMainLib())

	// 'xcore' unittest project
	maintest := denv.SetupDefaultCppTestProject("xbase_test", "github.com\\jurgen-kluft\\xcore")

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
