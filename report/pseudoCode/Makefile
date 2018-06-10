SOURCEDIR = ./report/pseudoCodes
BUILDDIR = latex-build

all:
	latexmk -outdir=$(BUILDDIR) -pdf -shell-escape pseudoCodes.tex

clean:
	latexmk -outdir=$(BUILDDIR) -c