
NAME = ddslam

TEXS=$(wildcard docs/*.tex)
PDFS=$(TEXS:%.tex=%.pdf)
LOGS=$(TEXS:%.tex=%.log)
AUXS=$(TEXS:%.tex=%.aux)

SRCS=$(wildcard  src/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)
UTLI_SRCS=$(wildcard  utils/*.cpp)

UTLIS=$(UTLI_SRCS:%.cpp=%)

LIBS = -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_video

all : $(PDFS) $(UTLIS)

$(PDFS) : %.pdf : %.tex
	xelatex -output-directory=docs $<

$(UTLIS) : % : %.cpp 
	@echo g++ -g $< -o $@
	@g++ -g $< $(LIBS) -o $@

clean:
	@echo Remove temporary files
	@rm -f $(PDFS) $(LOGS) $(AUXS) $(UTLIS)
	@find -name "*~" -type f -delete
	@find -name "*.flw" -type f -delete

debug_flow:
	gdb utils/flow

flow:
	utils/flow data/videos/720.mp4

echo:
	@echo LIBS:
	@echo $(LIBS)
	@echo UTLIS:
	@echo $(UTLIS)
	@echo UTLI_SRCS:
	@echo $(UTLI_SRCS)
	@echo TOOL_OBJS:
	@echo $(TOOL_OBJS)



#$(UTLIS) : $(TOOL_OBJS) 
#	g++ -g $< -o $@

#$(UTLIS_OBJS) : %.o : %.cpp
#	g++ -c -g $< -o $@
#TOOL_OBJS=$(UTLI_SRCS:%.cpp=%.o)
#rm -f *~ docs/*~ srcs/*~ utils/*~ html/*~
#UTLIS = $(basename $(UTLI_SRCS))
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
