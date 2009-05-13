
.SUFFIXES:
.SUFFIXES: .svg .pdf


all: presentation.pdf

SVGS = *.svg
SVG_PDFS = $(SVGS:.svg=.pdf)

.svg.pdf :
	inkscape --export-pdf=`basename $< .svg`.pdf $<


presentation.pdf: $(SVG_PDFS) presentation.tex
	pdflatex presentation.tex
	pdflatex presentation.tex
