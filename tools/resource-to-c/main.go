package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
)

type resourceToC struct {
	input, output *os.File
	variablename  string
}

type arrayInfo struct {
	x int
	y int
}

func main() {
	var filename string
	var output string
	var variablename string

	flag.StringVar(&filename, "f", "", "path to hankaku.txt")
	flag.StringVar(&output, "o", "", "path to output file")
	flag.StringVar(&variablename, "v", "", "variable name decleared in c")
	flag.Parse()

	fp, err := os.Open(filename)
	if err != nil {
		log.Fatal(err)
	}

	ofp, err := os.Create(output)
	if err != nil {
		log.Fatal(err)
	}
	convertToClangFile(resourceToC{input: fp, output: ofp, variablename: variablename})
}

func convertToClangFile(r resourceToC) {
	scanner := bufio.NewScanner(r.input)
	var eachLine string
	// readFirstLine
	if scanner.Scan() {
		eachLine = scanner.Text()
		r.output.WriteString(fmt.Sprintf("char %s[][%d] = {\n", r.variablename, len(eachLine)))
		convertLineToC(r, []byte(eachLine))
	} else {
		if err := scanner.Err(); err != nil {
			log.Fatal(err)
		}
	}

	for scanner.Scan() {
		eachLine := scanner.Text()
		convertLineToC(r, []byte(eachLine))
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	r.output.WriteString("};\n")
	defer closeFiles(r)
}

func convertLineToC(r resourceToC, line []byte) {
	r.output.WriteString("{")
	for i := 0; i < len(line); i++ {
		r.output.WriteString(fmt.Sprintf("0x%x,", line[i]))
	}
	r.output.WriteString("},\n")
}

func closeFiles(r resourceToC) {
	r.input.Close()
	r.output.Close()
}
