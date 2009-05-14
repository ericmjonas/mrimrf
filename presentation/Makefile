
.SUFFIXES:
.SUFFIXES: .svg .pdf


all: presentation.pdf

SVGS = $(wildcard *.svg)
SVG_PDFS = ${SVGS:.svg=.pdf}

.svg.pdf:
	inkscape --export-pdf=`basename $< .svg`.full.pdf $<
	pdfcrop `basename $< .svg`.full.pdf  `basename $< .svg`.pdf 

presentation.pdf:  presentation.tex $(SVG_PDFS) mrimrf.bib
	pdflatex presentation.tex
	bibtex presentation
	pdflatex presentation.tex
	pdflatex presentation.tex

clean: 
	rm presentation.pdf
	rm $(SVG_PDFS)
