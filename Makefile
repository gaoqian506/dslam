
NAME = ddslam

TEXS=$(wildcard docs/*.tex)
PDFS=$(TEXS:%.tex=%.pdf)
LOGS=$(TEXS:%.tex=%.log)
AUXS=$(TEXS:%.tex=%.aux)

SRCS=$(wildcard  src/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)
TOOL_SRCS=$(wildcard  utils/*.cpp)
TOOLS=$(TOOL_SRCS:%.cpp=%)

all : $(TOOLS) $(PDFS)

$(PDFS) : %.pdf : %.tex
	xelatex -output-directory=docs $<

$(APPS) : % : %.cpp
	g++ $< -o $@

clean:
	@echo Remove temporary files
	@rm -f $(PDFS) $(LOGS) $(AUXS) $(TOOLS)
	@find -name "*~" -type f -delete
	

#rm -f *~ docs/*~ srcs/*~ utils/*~ html/*~
#TOOLS = $(basename $(TOOL_SRCS))
#	echo $(TOOLS)
#	echo $(TOOL_SRCS)
#	echo $(NAMES)

# 
#TARGETS = $(NAME).pdf $(NAME).dvi
#
# $(TARGETS) : $(NAME).tex
#	latex $(NAME).tex
#	
#
#dvi:
#	xdvi $(NAME).dvi
#rm $(TARGETS) $(NAME).aux $(NAME).log
