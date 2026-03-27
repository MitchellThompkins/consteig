---
title: Development
---

# Development

## With Docker (recommended)

All build dependencies are packaged inside the dev container, and make is used as
a matter of convenience.

```bash
make container.pull
export MY_UID=$(id -u)
export MY_GID=$(id -g)
make container.make.build
make container.make.test
```

Any make target can be run inside the container via `container.make.<target>`.

The container can be dropped into with `make container.start`.

## Without Docker (native)

Required:

* gcc and/or clang (C++17 support)
* cmake (>= 3.13)
* make
* clang-format (for `make format`)

Optional:

* Octave (for regenerating test cases via `make generate-test-cases`)

```bash
make build
make test
```
