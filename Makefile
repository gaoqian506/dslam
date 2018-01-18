

TEXS=$(wildcard docs/*.tex)
PDFS=$(TEXS:%.tex=%.pdf)

BIBS=$(wildcard docs/*.bib)
BBLS=$(BIBS:%.bib=%.bbl)

SRCS=$(wildcard  src/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)
BIN_SRCS=$(wildcard  bin/*.cpp)
BINS=$(BIN_SRCS:%.cpp=%.bin)

LIBS = -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_video
INCLUDES = -Iinclude


nothing:

all : bbls pdfs bins

soba : docs/soba/soba.bbl docs/soba/soba.pdf
im2scene : docs/im2scene/im2scene.bbl docs/im2scene/im2scene.pdf

dof : docs/dof.bbl docs/dof.tex

uml : docs/uml.pdf

flow : bin/flow.bin

local : bin/local.bin

bbls : $(BBLS)

pdfs : $(PDFS)

bins : $(BINS)

objs : $(OBJS)

%.bbl : %.bib
	@echo ----making $@-----------------
	xelatex -output-directory=$(dir $<) $(basename $<).tex
	bibtex $(basename $<).aux
	xelatex -output-directory=$(dir $<) $(basename $<).tex
	xelatex -output-directory=$(dir $<) $(basename $<).tex
	pdftotext $(basename $<).pdf

%.pdf : %.tex
	xelatex -output-directory=$(dir $<) $<
	pdftotext $@


%.bin : %.cpp $(OBJS)
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
	find -name "*.bin" -type f -delete
	find -name "*.txt" -type f -delete
	find -name "*.o" -type f -delete

clean_flow:
	find -name "*.flw" -type f -delete

debug_flow:
	gdb bin/flow.bin

debug_local:
	gdb bin/local.bin

run_flow:
	bin/flow.bin data/videos/720.mp4

run_local:
	bin/local.bin

echo:
	@echo LIBS:
	@echo $(LIBS)
	@echo BINS:
	@echo $(BINS)
	@echo BIN_SRCS:
	@echo $(BIN_SRCS)
	@echo TOOL_OBJS:
	@echo $(TOOL_OBJS)
