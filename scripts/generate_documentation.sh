#!/bin/bash

# Documentation Generation Script
# Automatically generates comprehensive API documentation

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() { echo -e "${BLUE}[INFO]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }

OUTPUT_DIR="docs/generated"
FORMAT="html"
SERVE=false
DEPLOY=false

usage() {
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -o <dir>      Output directory (default: docs/generated)"
    echo "  -f <format>   Output format: html, latex, rtf (default: html)" 
    echo "  -s            Serve documentation locally after generation"
    echo "  -d            Deploy to GitHub Pages"
    echo "  -h            Show this help"
    echo ""
    echo "Examples:"
    echo "  $0                    # Generate HTML documentation"
    echo "  $0 -s                 # Generate and serve locally"
    echo "  $0 -f latex           # Generate LaTeX documentation"
    echo "  $0 -d                 # Generate and deploy to GitHub Pages"
    exit 1
}

# Parse command line arguments
while getopts "o:f:sdh" opt; do
    case $opt in
        o) OUTPUT_DIR="$OPTARG" ;;
        f) FORMAT="$OPTARG" ;;
        s) SERVE=true ;;
        d) DEPLOY=true ;;
        h) usage ;;
        *) usage ;;
    esac
done

echo "📚 SyLife Documentation Generation"
echo "=================================="

# Check if Doxygen is installed
if ! command -v doxygen &> /dev/null; then
    print_error "Doxygen not found. Please install doxygen and graphviz:"
    echo "  Ubuntu/Debian: sudo apt-get install doxygen graphviz"
    echo "  macOS: brew install doxygen graphviz"
    echo "  Windows: choco install doxygen.install graphviz"
    exit 1
fi

# Check if Graphviz is installed (for diagrams)
if ! command -v dot &> /dev/null; then
    print_warning "Graphviz not found. Diagrams will not be generated."
    echo "Install with: sudo apt-get install graphviz"
fi

# Create output directory
print_status "Creating output directory: $OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"

# Generate Doxyfile if it doesn't exist
if [[ ! -f "Doxyfile" ]]; then
    print_status "Generating Doxyfile configuration..."
    
    # Create a comprehensive Doxyfile
    cat > Doxyfile << 'EOF'
# SyLife API Documentation Configuration

#---------------------------------------------------------------------------
# Project related configuration options
#---------------------------------------------------------------------------
DOXYFILE_ENCODING      = UTF-8
PROJECT_NAME           = "SyLife"
PROJECT_NUMBER         = "2.0"
PROJECT_BRIEF          = "Modern Self-Designed Ecosystem Simulator"
PROJECT_LOGO           = 
OUTPUT_DIRECTORY       = docs/generated
CREATE_SUBDIRS         = NO
ALLOW_UNICODE_NAMES    = NO
OUTPUT_LANGUAGE        = English
BRIEF_MEMBER_DESC      = YES
REPEAT_BRIEF           = YES
ABBREVIATE_BRIEF       = "The $name class" \
                         "The $name widget" \
                         "The $name file" \
                         is \
                         provides \
                         specifies \
                         contains \
                         represents \
                         a \
                         an \
                         the
ALWAYS_DETAILED_SEC    = NO
INLINE_INHERITED_MEMB  = NO
FULL_PATH_NAMES        = YES
STRIP_FROM_PATH        = include/ \
                         modules/
STRIP_FROM_INC_PATH    = include/ \
                         modules/
SHORT_NAMES            = NO
JAVADOC_AUTOBRIEF      = YES
QT_AUTOBRIEF           = NO
MULTILINE_CPP_IS_BRIEF = NO
INHERIT_DOCS           = YES
SEPARATE_MEMBER_PAGES  = NO
TAB_SIZE               = 4
ALIASES                = 
OPTIMIZE_OUTPUT_FOR_C  = NO
OPTIMIZE_OUTPUT_JAVA   = NO
OPTIMIZE_FOR_FORTRAN   = NO
OPTIMIZE_OUTPUT_VHDL   = NO
EXTENSION_MAPPING      = 
MARKDOWN_SUPPORT       = YES
TOC_INCLUDE_HEADINGS   = 0
AUTOLINK_SUPPORT       = YES
BUILTIN_STL_SUPPORT    = YES
CPP_CLI_SUPPORT        = NO
SIP_SUPPORT            = NO
IDL_PROPERTY_SUPPORT   = YES
DISTRIBUTE_GROUP_DOC   = NO
GROUP_NESTED_COMPOUNDS = NO
SUBGROUPING            = YES
INLINE_GROUPED_CLASSES = NO
INLINE_SIMPLE_STRUCTS  = NO
TYPEDEF_HIDES_STRUCT   = NO
LOOKUP_CACHE_SIZE      = 0

#---------------------------------------------------------------------------
# Build related configuration options
#---------------------------------------------------------------------------
EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = NO
EXTRACT_PACKAGE        = NO
EXTRACT_STATIC         = YES
EXTRACT_LOCAL_CLASSES  = YES
EXTRACT_LOCAL_METHODS  = NO
EXTRACT_ANON_NSPACES   = NO
HIDE_UNDOC_MEMBERS     = NO
HIDE_UNDOC_CLASSES     = NO
HIDE_FRIEND_COMPOUNDS  = NO
HIDE_IN_BODY_DOCS      = NO
INTERNAL_DOCS          = NO
CASE_SENSE_NAMES       = NO
HIDE_SCOPE_NAMES       = NO
HIDE_COMPOUND_REFERENCE= NO
SHOW_INCLUDE_FILES     = YES
SHOW_GROUPED_MEMB_INC  = NO
FORCE_LOCAL_INCLUDES   = NO
INLINE_INFO            = YES
SORT_MEMBER_DOCS       = YES
SORT_BRIEF_DOCS        = NO
SORT_MEMBERS_CTORS_1ST = NO
SORT_GROUP_NAMES       = NO
SORT_BY_SCOPE_NAME     = NO
STRICT_PROTO_MATCHING  = NO
GENERATE_TODOLIST      = YES
GENERATE_TESTLIST      = YES
GENERATE_BUGLIST       = YES
GENERATE_DEPRECATEDLIST= YES
ENABLED_SECTIONS       = 
MAX_INITIALIZER_LINES  = 30
SHOW_USED_FILES        = YES
SHOW_FILES             = YES
SHOW_NAMESPACES        = YES
FILE_VERSION_FILTER    = 
LAYOUT_FILE            = 
CITE_BIB_FILES         = 

#---------------------------------------------------------------------------
# Configuration options related to warning and progress messages
#---------------------------------------------------------------------------
QUIET                  = NO
WARNINGS               = YES
WARN_IF_UNDOCUMENTED   = YES
WARN_IF_DOC_ERROR      = YES
WARN_NO_PARAMDOC       = NO
WARN_AS_ERROR          = NO
WARN_FORMAT            = "$file:$line: $text"
WARN_LOGFILE           = 

#---------------------------------------------------------------------------
# Configuration options related to the input files
#---------------------------------------------------------------------------
INPUT                  = include/ \
                         modules/ \
                         README.md \
                         DEVELOPER_ONBOARDING.md \
                         MODULAR_ARCHITECTURE.md
INPUT_ENCODING         = UTF-8
FILE_PATTERNS          = *.c \
                         *.cc \
                         *.cxx \
                         *.cpp \
                         *.c++ \
                         *.h \
                         *.hh \
                         *.hxx \
                         *.hpp \
                         *.h++ \
                         *.md
RECURSIVE              = YES
EXCLUDE                = build* \
                         .git \
                         node_modules \
                         thirdparty
EXCLUDE_SYMLINKS       = NO
EXCLUDE_PATTERNS       = */build*/* \
                         */.git/* \
                         */test*/* \
                         *_test.* \
                         *test_*
EXCLUDE_SYMBOLS        = 
EXAMPLE_PATH           = examples/ \
                         docs/examples/
EXAMPLE_PATTERNS       = *.cpp \
                         *.h
EXAMPLE_RECURSIVE      = YES
IMAGE_PATH             = docs/images/
INPUT_FILTER           = 
FILTER_PATTERNS        = 
FILTER_SOURCE_FILES    = NO
FILTER_SOURCE_PATTERNS = 
USE_MDFILE_AS_MAINPAGE = README.md

#---------------------------------------------------------------------------
# Configuration options related to source browsing
#---------------------------------------------------------------------------
SOURCE_BROWSER         = YES
INLINE_SOURCES         = NO
STRIP_CODE_COMMENTS    = YES
REFERENCED_BY_RELATION = NO
REFERENCES_RELATION    = NO
REFERENCES_LINK_SOURCE = YES
SOURCE_TOOLTIPS        = YES
USE_HTAGS              = NO
VERBATIM_HEADERS       = YES
CLANG_ASSISTED_PARSING = NO
CLANG_OPTIONS          = 

#---------------------------------------------------------------------------
# Configuration options related to the alphabetical class index
#---------------------------------------------------------------------------
ALPHABETICAL_INDEX     = YES
COLS_IN_ALPHA_INDEX    = 5
IGNORE_PREFIX          = 

#---------------------------------------------------------------------------
# Configuration options related to the HTML output
#---------------------------------------------------------------------------
GENERATE_HTML          = YES
HTML_OUTPUT            = html
HTML_FILE_EXTENSION    = .html
HTML_HEADER            = 
HTML_FOOTER            = 
HTML_STYLESHEET        = 
HTML_EXTRA_STYLESHEET  = 
HTML_EXTRA_FILES       = 
HTML_COLORSTYLE_HUE    = 220
HTML_COLORSTYLE_SAT    = 100
HTML_COLORSTYLE_GAMMA  = 80
HTML_TIMESTAMP         = YES
HTML_DYNAMIC_SECTIONS  = NO
HTML_INDEX_NUM_ENTRIES = 100
GENERATE_DOCSET        = NO
DOCSET_FEEDNAME        = "Doxygen generated docs"
DOCSET_BUNDLE_ID       = org.doxygen.Project
DOCSET_PUBLISHER_ID    = org.doxygen.Publisher
DOCSET_PUBLISHER_NAME  = Publisher
GENERATE_HTMLHELP      = NO
CHM_FILE               = 
HHC_LOCATION           = 
GENERATE_CHI           = NO
CHM_INDEX_ENCODING     = 
BINARY_TOC             = NO
TOC_EXPAND             = NO
GENERATE_QHP           = NO
QCH_FILE               = 
QHP_NAMESPACE          = org.doxygen.Project
QHP_VIRTUAL_FOLDER     = doc
QHP_CUST_FILTER_NAME   = 
QHP_CUST_FILTER_ATTRS  = 
QHP_SECT_FILTER_ATTRS  = 
QHG_LOCATION           = 
GENERATE_ECLIPSEHELP   = NO
ECLIPSE_DOC_ID         = org.doxygen.Project
DISABLE_INDEX          = NO
GENERATE_TREEVIEW      = NO
ENUM_VALUES_PER_LINE   = 4
TREEVIEW_WIDTH         = 250
EXT_LINKS_IN_WINDOW    = NO
FORMULA_FONTSIZE       = 10
FORMULA_TRANSPARENT    = YES
USE_MATHJAX            = NO
MATHJAX_FORMAT         = HTML-CSS
MATHJAX_RELPATH        = http://cdn.mathjax.org/mathjax/latest
MATHJAX_EXTENSIONS     = 
MATHJAX_CODEFILE       = 
SEARCHENGINE           = YES
SERVER_BASED_SEARCH    = NO
EXTERNAL_SEARCH        = NO
SEARCHENGINE_URL       = 
SEARCHDATA_FILE        = searchdata.xml
EXTERNAL_SEARCH_ID     = 
EXTRA_SEARCH_MAPPINGS  = 

#---------------------------------------------------------------------------
# Configuration options related to the LaTeX output
#---------------------------------------------------------------------------
GENERATE_LATEX         = NO
LATEX_OUTPUT           = latex
LATEX_CMD_NAME         = latex
MAKEINDEX_CMD_NAME     = makeindex
COMPACT_LATEX          = NO
PAPER_TYPE             = a4
EXTRA_PACKAGES         = 
LATEX_HEADER           = 
LATEX_FOOTER           = 
LATEX_EXTRA_STYLESHEET = 
LATEX_EXTRA_FILES      = 
PDF_HYPERLINKS         = YES
USE_PDFLATEX           = YES
LATEX_BATCHMODE        = NO
LATEX_HIDE_INDICES     = NO
LATEX_SOURCE_CODE      = NO
LATEX_BIB_STYLE        = plain

#---------------------------------------------------------------------------
# Configuration options related to the RTF output
#---------------------------------------------------------------------------
GENERATE_RTF           = NO
RTF_OUTPUT             = rtf
COMPACT_RTF            = NO
RTF_HYPERLINKS         = NO
RTF_STYLESHEET_FILE    = 
RTF_EXTENSIONS_FILE    = 
RTF_SOURCE_CODE        = NO

#---------------------------------------------------------------------------
# Configuration options related to the man page output
#---------------------------------------------------------------------------
GENERATE_MAN           = NO
MAN_OUTPUT             = man
MAN_EXTENSION          = .3
MAN_SUBDIR             = 
MAN_LINKS              = NO

#---------------------------------------------------------------------------
# Configuration options related to the XML output
#---------------------------------------------------------------------------
GENERATE_XML           = NO
XML_OUTPUT             = xml
XML_PROGRAMLISTING     = YES

#---------------------------------------------------------------------------
# Configuration options related to the DOCBOOK output
#---------------------------------------------------------------------------
GENERATE_DOCBOOK       = NO
DOCBOOK_OUTPUT         = docbook
DOCBOOK_PROGRAMLISTING = NO

#---------------------------------------------------------------------------
# Configuration options for the AutoGen Definitions output
#---------------------------------------------------------------------------
GENERATE_AUTOGEN_DEF   = NO

#---------------------------------------------------------------------------
# Configuration options related to the Perl module output
#---------------------------------------------------------------------------
GENERATE_PERLMOD       = NO
PERLMOD_LATEX          = NO
PERLMOD_PRETTY         = YES
PERLMOD_MAKEVAR_PREFIX = 

#---------------------------------------------------------------------------
# Configuration options related to the preprocessor
#---------------------------------------------------------------------------
ENABLE_PREPROCESSING   = YES
MACRO_EXPANSION        = NO
EXPAND_ONLY_PREDEF     = NO
SEARCH_INCLUDES        = YES
INCLUDE_PATH           = include/ \
                         modules/
INCLUDE_FILE_PATTERNS  = 
PREDEFINED             = SYLIFE_API= \
                         SYLIFE_EXPORT= \
                         SIV3D_TARGET_LINUX
EXPAND_AS_DEFINED      = 
SKIP_FUNCTION_MACROS   = YES

#---------------------------------------------------------------------------
# Configuration options related to external references
#---------------------------------------------------------------------------
TAGFILES               = 
GENERATE_TAGFILE       = 
ALLEXTERNALS           = NO
EXTERNAL_GROUPS        = YES
EXTERNAL_PAGES         = YES
PERL_PATH              = /usr/bin/perl

#---------------------------------------------------------------------------
# Configuration options related to the dot tool
#---------------------------------------------------------------------------
CLASS_DIAGRAMS         = YES
MSCGEN_PATH            = 
DIA_PATH               = 
HIDE_UNDOC_RELATIONS   = YES
HAVE_DOT               = YES
DOT_NUM_THREADS        = 0
DOT_FONTNAME           = Helvetica
DOT_FONTSIZE           = 10
DOT_FONTPATH           = 
CLASS_GRAPH            = YES
COLLABORATION_GRAPH    = YES
GROUP_GRAPHS           = YES
UML_LOOK               = NO
UML_LIMIT_NUM_FIELDS   = 10
TEMPLATE_RELATIONS     = NO
INCLUDE_GRAPH          = YES
INCLUDED_BY_GRAPH      = YES
CALL_GRAPH             = NO
CALLER_GRAPH           = NO
GRAPHICAL_HIERARCHY    = YES
DIRECTORY_GRAPH        = YES
DOT_IMAGE_FORMAT       = png
INTERACTIVE_SVG        = NO
DOT_PATH               = 
DOTFILE_DIRS           = 
MSCFILE_DIRS           = 
DIAFILE_DIRS           = 
PLANTUML_JAR_PATH      = 
PLANTUML_CFG_FILE      = 
PLANTUML_INCLUDE_PATH  = 
DOT_GRAPH_MAX_NODES    = 50
MAX_DOT_GRAPH_DEPTH    = 0
DOT_TRANSPARENT        = NO
DOT_MULTI_TARGETS      = NO
GENERATE_LEGEND        = YES
DOT_CLEANUP            = YES
EOF

    print_success "Doxyfile created"
fi

# Update Doxyfile for current generation
print_status "Configuring documentation generation..."

# Set output format
case "$FORMAT" in
    "html")
        sed -i 's/GENERATE_HTML.*/GENERATE_HTML = YES/' Doxyfile
        sed -i 's/GENERATE_LATEX.*/GENERATE_LATEX = NO/' Doxyfile
        sed -i 's/GENERATE_RTF.*/GENERATE_RTF = NO/' Doxyfile
        ;;
    "latex")
        sed -i 's/GENERATE_HTML.*/GENERATE_HTML = NO/' Doxyfile
        sed -i 's/GENERATE_LATEX.*/GENERATE_LATEX = YES/' Doxyfile
        sed -i 's/GENERATE_RTF.*/GENERATE_RTF = NO/' Doxyfile
        ;;
    "rtf")
        sed -i 's/GENERATE_HTML.*/GENERATE_HTML = NO/' Doxyfile
        sed -i 's/GENERATE_LATEX.*/GENERATE_LATEX = NO/' Doxyfile
        sed -i 's/GENERATE_RTF.*/GENERATE_RTF = YES/' Doxyfile
        ;;
    *)
        print_error "Unsupported format: $FORMAT"
        exit 1
        ;;
esac

# Set output directory
sed -i "s|OUTPUT_DIRECTORY.*|OUTPUT_DIRECTORY = $OUTPUT_DIR|" Doxyfile

# Generate documentation
print_status "Generating documentation in $FORMAT format..."
doxygen Doxyfile

if [[ $? -eq 0 ]]; then
    print_success "Documentation generated successfully!"
else
    print_error "Documentation generation failed"
    exit 1
fi

# Generate additional documentation
print_status "Generating additional documentation..."

# Create API overview
if [[ ! -d "$OUTPUT_DIR/$FORMAT" ]]; then
    mkdir -p "$OUTPUT_DIR/$FORMAT"
fi

cat > "$OUTPUT_DIR/README.md" << 'EOF'
# SyLife API Documentation

This directory contains the automatically generated API documentation for SyLife.

## Contents

- `html/` - HTML documentation (browse with web browser)
- `latex/` - LaTeX documentation (for PDF generation)
- `rtf/` - RTF documentation (for word processors)

## Quick Start

1. **Browse HTML Documentation**: Open `html/index.html` in your web browser
2. **Generate PDF from LaTeX**: `cd latex && make` (requires LaTeX installation)
3. **View RTF**: Open `rtf/refman.rtf` in any word processor

## Documentation Structure

- **Classes**: All C++ classes with their methods and members
- **Files**: Source file documentation
- **Namespaces**: Code organization
- **Examples**: Code examples and tutorials
- **Modules**: Architecture components

## Regenerating Documentation

To regenerate this documentation:

```bash
./scripts/generate_documentation.sh
```

For other formats:

```bash
./scripts/generate_documentation.sh -f latex
./scripts/generate_documentation.sh -f rtf
```

## Getting Started

1. Start with the [Main Page](html/index.html)
2. Browse [Classes](html/annotated.html) for API reference
3. Check [Examples](html/examples.html) for usage patterns
4. Read [Modules](html/modules.html) for architecture overview

---

Generated: $(date)
EOF

# Generate sitemap for HTML documentation
if [[ "$FORMAT" == "html" ]] && [[ -d "$OUTPUT_DIR/html" ]]; then
    print_status "Generating sitemap..."
    
    find "$OUTPUT_DIR/html" -name "*.html" | sort > "$OUTPUT_DIR/html/sitemap.txt"
    print_success "Sitemap generated"
fi

# Generate coverage report if available
if [[ -f "build/coverage.info" ]]; then
    print_status "Generating test coverage documentation..."
    genhtml build/coverage.info --output-directory "$OUTPUT_DIR/coverage" 2>/dev/null || true
fi

# Serve documentation locally if requested
if [[ "$SERVE" == true ]] && [[ "$FORMAT" == "html" ]]; then
    print_status "Serving documentation locally..."
    
    if command -v python3 &> /dev/null; then
        print_success "Documentation available at http://localhost:8080"
        print_status "Press Ctrl+C to stop the server"
        cd "$OUTPUT_DIR/html"
        python3 -m http.server 8080
    elif command -v python &> /dev/null; then
        print_success "Documentation available at http://localhost:8080"
        print_status "Press Ctrl+C to stop the server"
        cd "$OUTPUT_DIR/html"
        python -m http.server 8080
    else
        print_warning "Python not found. Cannot serve documentation."
        print_status "Open $OUTPUT_DIR/html/index.html in your browser"
    fi
fi

# Deploy to GitHub Pages if requested
if [[ "$DEPLOY" == true ]] && [[ "$FORMAT" == "html" ]]; then
    print_status "Deploying to GitHub Pages..."
    
    if ! command -v gh &> /dev/null; then
        print_error "GitHub CLI not found. Please install gh command"
        exit 1
    fi
    
    # Check if gh-pages branch exists
    if git rev-parse --verify gh-pages >/dev/null 2>&1; then
        git checkout gh-pages
    else
        git checkout --orphan gh-pages
        git rm -rf .
    fi
    
    # Copy documentation
    cp -r "$OUTPUT_DIR/html/"* .
    
    # Create .nojekyll to bypass Jekyll processing
    touch .nojekyll
    
    # Commit and push
    git add .
    git commit -m "📚 Update API documentation

Generated: $(date)

🤖 Generated with [Claude Code](https://claude.ai/code)

Co-Authored-By: Claude <noreply@anthropic.com>"
    git push origin gh-pages
    
    # Return to original branch
    git checkout -
    
    print_success "Documentation deployed to GitHub Pages"
    print_status "Available at: https://$(git config --get remote.origin.url | sed 's/.*github.com[:/]\([^/]*\/[^/]*\).*/\1/' | sed 's/\.git$//')/github.io/$(basename $(pwd))"
fi

# Generate final report
echo ""
echo "=========================================="
print_success "📚 Documentation Generation Complete!"
echo "=========================================="
echo ""
echo "📊 Generation Summary:"
echo "   Format: $FORMAT"
echo "   Output: $OUTPUT_DIR"
echo "   Files: $(find "$OUTPUT_DIR" -type f | wc -l)"
echo "   Size: $(du -sh "$OUTPUT_DIR" | cut -f1)"
echo ""
echo "📂 Generated Documentation:"
if [[ "$FORMAT" == "html" ]]; then
    echo "   Main Page: $OUTPUT_DIR/html/index.html"
    echo "   Classes: $OUTPUT_DIR/html/annotated.html"
    echo "   Files: $OUTPUT_DIR/html/files.html"
fi
echo "   README: $OUTPUT_DIR/README.md"
echo ""
echo "🚀 Quick Commands:"
echo "   Open in browser: open $OUTPUT_DIR/html/index.html"
echo "   Serve locally: python3 -m http.server 8080 -d $OUTPUT_DIR/html"
echo "   Deploy to GitHub: $0 -d"
echo ""
print_success "Documentation ready for use! 📖✨"