
.PHONY: lint
lint:
	find src -iname *.h -o -iname *.cc | xargs clang-format -n -Werror

.PHONY: fix
fix:
	find src -iname *.h -o -iname *.cc | xargs clang-format -i