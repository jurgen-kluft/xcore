package xcore

import (
	"github.com/jurgen-kluft/xbase/package"
	"github.com/jurgen-kluft/xcode/denv"
	"github.com/jurgen-kluft/xentry/package"
	"github.com/jurgen-kluft/xunittest/package"
)

// GetPackage returns the package object of 'xcore'
func GetPackage() *denv.Package {
	// Dependencies
	unittestpkg := xunittest.GetPackage()
	entrypkg := xentry.GetPackage()
	xbasepkg := xbase.GetPackage()

	// The main (xcore) package
	mainpkg := denv.NewPackage("xcore")
	mainpkg.AddPackage(unittestpkg)
	mainpkg.AddPackage(entrypkg)
	mainpkg.AddPackage(xbasepkg)

	// 'xcore' library
	mainlib := denv.SetupDefaultCppLibProject("xcore", "github.com\\jurgen-kluft\\xcore")
	mainlib.Dependencies = append(mainlib.Dependencies, xbasepkg.GetMainLib())

	// 'xcore' unittest project
	maintest := denv.SetupDefaultCppTestProject("xcore_test", "github.com\\jurgen-kluft\\xcore")
	maintest.Dependencies = append(maintest.Dependencies, unittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, entrypkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xbasepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
