
NAME = ddslam

TEXS=$(wildcard docs/*.tex)
PDFS=$(TEXS:%.tex=%.pdf)

BIBS=$(wildcard docs/*.bib)
BBLS=$(BIBS:%.bib=%.bbl)

SRCS=$(wildcard  src/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)
UTL_SRCS=$(wildcard  utils/*.cpp)
UTLS=$(UTL_SRCS:%.cpp=%.utl)

LIBS = -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_video
INCLUDES = -Iinclude


nothing:

all : bbls pdfs utls 

soba : docs/soba/soba.bbl docs/soba/soba.pdf

dof : docs/dof.bbl docs/dof.tex

uml : docs/uml.pdf

flow : utils/flow.utl

local : utils/local.utl

bbls : $(BBLS)

pdfs : $(PDFS)

utls : $(UTLS)

%.bbl : %.bib
	@echo ----making $@-----------------
	xelatex -output-directory=$(dir $<) $(basename $<).tex
	bibtex $(basename $<).aux
	xelatex -output-directory=$(dir $<) $(basename $<).tex
	xelatex -output-directory=$(dir $<) $(basename $<).tex

%.pdf : %.tex
	xelatex -output-directory=$(dir $<) $<


%.utl : %.cpp $(OBJS)
	g++ -g $(OBJS) $< $(INCLUDES) $(LIBS) -o $@

%.o : %.cpp
	g++ -c -g $< $(INCLUDES) $(LIBS) -o $@

clean: clean_flow
	find -name "*~" -type f -delete
	find -name "*.aux" -type f -delete
	find -name "*.log" -type f -delete
	find -name "*.bbl" -type f -delete
	find -name "*.blg" -type f -delete
	find -name "*.pdf" -type f -delete
	find -name "*.utl" -type f -delete
	find -name "*.txt" -type f -delete
	find -name "*.o" -type f -delete

clean_flow:
	find -name "*.flw" -type f -delete

debug_flow:
	gdb utils/flow.utl

run_flow:
	utils/flow.utl data/videos/720.mp4

echo:
	@echo LIBS:
	@echo $(LIBS)
	@echo UTILS:
	@echo $(UTILS)
	@echo UTLI_SRCS:
	@echo $(UTLI_SRCS)
	@echo TOOL_OBJS:
	@echo $(TOOL_OBJS)

