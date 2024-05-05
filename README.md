This repo is meant to show the current challenges with the Bazel IntellIj plugin and
the [Golang cgo module](https://pkg.go.dev/cmd/cgo). It was run on a MacOS laptop.

Try opening this repo with GoLand or IntelliJ Ultimate Edition. Set a breakpoint one of the lines. You'll notice that
nothing triggers.

The problem turns out to be related to [rules_go](https://github.com/bazelbuild/rules_go/pull/3011) that strips out
absolute path line directives to support build hermiticity. The problem however
is that the Golang debugger, Delve, is unable to provide source code references for work.


## Workaround

Delve provides a solution to these issues with
the [path substitution](https://github.com/go-delve/delve/blob/master/Documentation/cli/substitutepath.md)
configuration. We can add a file
such as `~/.dlv/config.yml` with the following YAML:

```yaml
substitute-path:
  - { from: "_main/", to: "/Users/rogerhu/projects/sample-cgo-repo" }
```

We can check by installing Delve locally:

```
brew install delve
```

And running:

```
bazel build //:test
dlv exec bazel-out/darwin_arm64-dbg/bin/test_/test

```

You can then see:

```
(dlv) b main.main
Breakpoint 1 set at 0x101022fd0 for main.main() ./tst.go:11
(dlv) c
> main.main() ./tst.go:11 (hits goroutine(1):1 total:1) (PC: 0x101022fd0)
     6:	*/
     7:	import "C"
     8:
     9:	import "fmt"
    10:
=>  11:	func main() {
    12:		fmt.Println("hello")
    13:		result := C.keychain_get(C.CString("engflow"))
    14:		fmt.Printf(C.GoString(result))
    15:	}

```

However, this logic only works with the Delve command-line tool, not with the Bazel IntelliJ plugin. VScode currently
supports a variation of this logic [here](https://github.com/golang/vscode-go/blob/master/docs/debugging.md).
The [full logic](https://github.com/go-delve/delve/blob/master/pkg/locspec/locations.go#L554) should follow how it's
currently done in Delve.