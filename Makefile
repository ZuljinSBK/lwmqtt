fmt:
	clang-format -i src/* -style="{BasedOnStyle: Google, ColumnLimit: 120}"
	clang-format -i test/* -style="{BasedOnStyle: Google, ColumnLimit: 120}"
