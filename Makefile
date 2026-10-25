.PHONY: check test sanitize verify

check:
	bash scripts/compile_all.sh

test:
	bash scripts/test_examples.sh

sanitize:
	bash scripts/test_sanitizers.sh

verify: check test sanitize
