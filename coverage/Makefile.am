coverage:
	$(LCOV) -d $(top_builddir) -c -o full.coverage
	$(LCOV) -e full.coverage *src/* -o snippets.coverage
	$(GENHTML) --show-details -t "Tests Coverage" --legend snippets.coverage -o report

clean-local:
	rm -rf report

CLEANFILES = \
	*.coverage

