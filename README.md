This repo is meant to show the current challenges with the Bazel IntellIj plugin and
the [Golang cgo module](https://pkg.go.dev/cmd/cgo). It was run on a MacOS laptop.

Try opening this repo with GoLand or IntelliJ Ultimate Edition. Set a breakpoint one of the lines. You'll notice that
nothing triggers.

The problem turns out to be related to [rules_go](https://github.com/bazelbuild/rules_go/pull/3011) that strips out
absolute path line directives to support build hermiticity. The problem however
is that the Golang debugger, Delve, is unable to provide source code references for work.

Delve provides a solution to these issues with
the [path substitution](https://github.com/go-delve/delve/blob/master/Documentation/cli/substitutepath.md)
configuration. We can add a file
such as `~/.dlv/config.yml` with the following YAML:

```yaml
substitute-path:
  - { from: "_main/", to: "/Users/rogerhu/projects/sample-cgo-repo" }
```

However, this logic only works with the Delve command-line tool, not with the Bazel IntelliJ plugin. VScode currently
supports a variation of this logic [here](https://github.com/golang/vscode-go/blob/master/docs/debugging.md).
The [full logic](https://github.com/go-delve/delve/blob/master/pkg/locspec/locations.go#L554) should follow how it's
currently done in Delve.