package main

import (
	"flag"
	"fmt"
	"strings"

	"github.com/tableauio/tableau/proto/tableaupb"
	"google.golang.org/protobuf/compiler/protogen"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/descriptorpb"
)

const version = "0.1.0"

var namespace *string

const pcExt = "pc" // protoconf file extension
const pbExt = "pb" // protobuf file extension

func main() {
	var flags flag.FlagSet
	namespace = flags.String("namespace", "tableau", "tableau namespace")
	flag.Parse()

	protogen.Options{
		ParamFunc: flags.Set,
	}.Run(func(gen *protogen.Plugin) error {
		for _, f := range gen.Files {
			if !f.Generate {
				continue
			}

			opts := f.Desc.Options().(*descriptorpb.FileOptions)
			workbook := proto.GetExtension(opts, tableaupb.E_Workbook).(*tableaupb.WorkbookOptions)
			if workbook == nil {
				continue
			}

			generateHppFile(gen, f)
			generateCppFile(gen, f)
		}
		return nil
	})
}

func protocVersion(gen *protogen.Plugin) string {
	v := gen.Request.GetCompilerVersion()
	if v == nil {
		return "(unknown)"
	}
	var suffix string
	if s := v.GetSuffix(); s != "" {
		suffix = "-" + s
	}
	return fmt.Sprintf("v%d.%d.%d%s", v.GetMajor(), v.GetMinor(), v.GetPatch(), suffix)
}

// generateHppFile generates loader files related to protoconf files.
func generateHppFile(gen *protogen.Plugin, file *protogen.File) *protogen.GeneratedFile {
	filename := file.GeneratedFilenamePrefix + "." + pcExt + ".h"
	g := gen.NewGeneratedFile(filename, file.GoImportPath)
	generateFileHeader(gen, file, g)
	generateHppFileContent(gen, file, g)
	return g
}

// generateCppFile generates loader files related to protoconf files.
func generateCppFile(gen *protogen.Plugin, file *protogen.File) *protogen.GeneratedFile {
	filename := file.GeneratedFilenamePrefix + "." + pcExt + ".cc"
	g := gen.NewGeneratedFile(filename, file.GoImportPath)
	generateFileHeader(gen, file, g)
	generateCppFileContent(gen, file, g)
	return g
}

func generateFileHeader(gen *protogen.Plugin, file *protogen.File, g *protogen.GeneratedFile) {
	g.P("// Code generated by protoc-gen-cpp-tableau-loader. DO NOT EDIT.")
	g.P("// versions:")
	g.P("// - protoc-gen-cpp-tableau-loader v", version)
	g.P("// - protoc                        ", protocVersion(gen))
	if file.Proto.GetOptions().GetDeprecated() {
		g.P("// ", file.Desc.Path(), " is a deprecated file.")
	} else {
		g.P("// source: ", file.Desc.Path())
	}
	g.P()
}

// generateHppFileContent generates type definitions.
func generateHppFileContent(gen *protogen.Plugin, file *protogen.File, g *protogen.GeneratedFile) {
	g.P("#pragma once")
	g.P("#include <string>")
	g.P()
	g.P(`#include "`, "hub.", pcExt, `.h"`)
	g.P(`#include "`, file.GeneratedFilenamePrefix, ".", pbExt, `.h"`)
	g.P()

	g.P("namespace ", *namespace, " {")
	for _, message := range file.Messages {
		opts := message.Desc.Options().(*descriptorpb.MessageOptions)
		worksheet := proto.GetExtension(opts, tableaupb.E_Worksheet).(*tableaupb.WorksheetOptions)
		if worksheet != nil {
			genHppMessage(gen, file, g, message)
		}
	}
	g.P("}  // namespace ", *namespace)
}

// genHppMessage generates a message definition.
func genHppMessage(gen *protogen.Plugin, file *protogen.File, g *protogen.GeneratedFile, message *protogen.Message) {
	pkg := string(file.Desc.Package())
	cppFullName := strings.ReplaceAll(pkg, ".", "::") + "::" + string(message.Desc.Name())

	g.P("class ", message.Desc.Name(), " : public Messager {")
	g.P(" public:")
	g.P("  static const std::string& Name() { return kProtoName; };")
	g.P("  const ", cppFullName, "& Get() const { return data_; };")
	g.P("  virtual bool Load(const std::string& dir, Format fmt) override;")
	g.P()
	g.P(" private:")
	g.P("  static const std::string kProtoName;")
	g.P("  ", cppFullName, " data_;")
	g.P("};")
	g.P()
}

// generateCppFileContent generates type implementations.
func generateCppFileContent(gen *protogen.Plugin, file *protogen.File, g *protogen.GeneratedFile) {
	g.P(`#include "`, file.GeneratedFilenamePrefix, ".", pcExt, `.h"`)
	g.P()

	g.P("namespace ", *namespace, " {")
	for _, message := range file.Messages {
		opts := message.Desc.Options().(*descriptorpb.MessageOptions)
		worksheet := proto.GetExtension(opts, tableaupb.E_Worksheet).(*tableaupb.WorksheetOptions)
		if worksheet != nil {
			genCppMessage(gen, file, g, message)
		}
	}
	g.P("}  // namespace ", *namespace)
}

// genCppMessage generates a message implementation.
func genCppMessage(gen *protogen.Plugin, file *protogen.File, g *protogen.GeneratedFile, message *protogen.Message) {
	g.P("const std::string ", message.Desc.Name(), "::kProtoName = ", `"`, message.Desc.Name(), `";`)
	g.P("bool ", message.Desc.Name(), "::Load(const std::string& dir, Format fmt) { return LoadMessage(dir, data_, fmt); }")
	g.P()
}