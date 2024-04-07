outDir=out
variantDirs=$(shell find $(outDir) -type d -maxdepth 1  -not  -name "$(outDir)" -print)
buildBase=ascon-build-base.py
buildFile=ascon-build.py
testFile=ascon-test.py
PYTHON=python3
testResultFiles=$(addsuffix /testResult.txt, $(variantDirs))

.PHONY: clean, help, print, pre, build, testall




testall : build, $(testResultFiles)
	@echo "tested all $(testResultFiles)"


$(testResultFiles): %/testResult.txt : %/$(testFile)
	$(PYTHON) %/$(testFile) >> %/teStResult.txt
	
pre:
	mkdir -p  ./$(outDir)
	cp -r variants/* $(outDir)
	pip install requirements.txt

build: pre
	./build.sh
		




clean : 
	rm -rf out/

print :
	@echo "variantDirs"
	@echo $(variantDirs)
	
	@echo "dest files"
	
	@echo $(dstFiles)

