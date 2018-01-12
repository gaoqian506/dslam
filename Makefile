
NAME = ddslam

TEXS=$(wildcard docs/*.tex)
PDFS=$(TEXS:%.tex=%.pdf)
LOGS=$(TEXS:%.tex=%.log)
AUXS=$(TEXS:%.tex=%.aux)

all : $(PDFS)

$(PDFS) : %.pdf : %.tex
	xelatex -output-directory=docs $<

clean:
	rm $(PDFS) $(LOGS) $(AUXS)



#TARGETS = $(NAME).pdf $(NAME).dvi
#
# $(TARGETS) : $(NAME).tex
#	latex $(NAME).tex
#	
#
#dvi:
#	xdvi $(NAME).dvi
#rm $(TARGETS) $(NAME).aux $(NAME).log
