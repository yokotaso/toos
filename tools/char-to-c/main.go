package main

import (
  "bufio"
  "fmt"
  "flag"
  "os"
  "log"
  "strings"
  "math"
  "errors"
)

type files struct {
   input, output *os.File
}

func main() {
    var filename string
    var output string
    flag.StringVar(&filename, "f", "", "path to hankaku.txt")
    flag.StringVar(&output, "o", "", "path to output file")
    flag.Parse()


    fp, err := os.Open(filename)
    if err != nil {
        log.Fatal(err)
    }

    ofp, err := os.Create(output)
    if err != nil {
        log.Fatal(err)
    }
    convertToClangFile(files{input: fp, output:ofp})
}

func convertToClangFile(f files) {
    f.output.WriteString("char hankaku[] = {\n")
    scanner := bufio.NewScanner(f.input)

    for scanner.Scan() {
        eachLine := scanner.Text()
        trimmed := strings.TrimSpace(eachLine)
        if trimmed == "" {
            continue
        } else if strings.HasPrefix(trimmed, "char") {
            f.output.WriteString(fmt.Sprintf("\n/** %s **/\n", trimmed))
        } else if eachLine != "" {
            f.output.WriteString(fmt.Sprintf("0x%x,", convertLineToHex([]byte(trimmed))))
        }
    }

    if err := scanner.Err(); err != nil {
       log.Fatal(err)
    }

    f.output.WriteString("};\n")
    closeFiles(f)
}
func convertLineToHex(line []byte) int {
    hex := 0.0
    length := len(line)
    for i := 0; i < length; i++ {
       if line[i] == '.' {
           hex = hex + 0
       } else if line[i] == '*'  {
           hex = hex + math.Pow(2, float64(length - i - 1))
       } else {
          log.Fatal(errors.New(fmt.Sprintf("Invalid char(%s) / %s", string(line[i]))))
       }
    }
    return int(hex)
}
func closeFiles(f files) {
    f.input.Close()
    f.output.Close()
}
