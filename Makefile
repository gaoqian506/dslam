
NAME = ddslam

TEXS=$(wildcard docs/*.tex)
PDFS=$(TEXS:%.tex=%.pdf)
LOGS=$(TEXS:%.tex=%.log)
AUXS=$(TEXS:%.tex=%.aux)

SRCS=$(wildcard  src/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)
UTLI_SRCS=$(wildcard  utils/*.cpp)
UTILS=$(UTLI_SRCS:%.cpp=%)

LIBS = -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_video

all : $(PDFS) $(UTILS)

pdfs : $(PDFS)

utils : $(UTILS)

$(PDFS) : %.pdf : %.tex
	xelatex -output-directory=docs $<

$(UTILS) : % : %.cpp 
	@echo g++ -g $< -o $@
	@g++ -g $< $(LIBS) -o $@

clean:
	@echo Remove temporary files
	@rm -f $(PDFS) $(LOGS) $(AUXS) $(UTILS)
	@find -name "*~" -type f -delete
	@find -name "*.flw" -type f -delete

debug_flow:
	gdb utils/flow

flow:
	utils/flow data/videos/720.mp4

echo:
	@echo LIBS:
	@echo $(LIBS)
	@echo UTILS:
	@echo $(UTILS)
	@echo UTLI_SRCS:
	@echo $(UTLI_SRCS)
	@echo TOOL_OBJS:
	@echo $(TOOL_OBJS)



#$(UTILS) : $(TOOL_OBJS) 
#	g++ -g $< -o $@

#$(UTILS_OBJS) : %.o : %.cpp
#	g++ -c -g $< -o $@
#TOOL_OBJS=$(UTLI_SRCS:%.cpp=%.o)
#rm -f *~ docs/*~ srcs/*~ utils/*~ html/*~
#UTILS = $(basename $(UTLI_SRCS))
#	@echo NAMES:
#	@echo $(NAMES)

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
