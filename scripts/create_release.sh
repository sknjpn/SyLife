#!/bin/bash

# Automated Release Creation Script
# Creates semantic versioned releases with changelog generation

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

RELEASE_TYPE="patch"
PRERELEASE=false
DRY_RUN=false
SKIP_TESTS=false

usage() {
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -t <type>     Release type: major, minor, patch (default: patch)"
    echo "  -p            Create pre-release"
    echo "  -d            Dry run (don't create actual release)"
    echo "  -s            Skip tests"
    echo "  -h            Show this help"
    echo ""
    echo "Examples:"
    echo "  $0                    # Create patch release (1.0.0 -> 1.0.1)"
    echo "  $0 -t minor           # Create minor release (1.0.0 -> 1.1.0)"
    echo "  $0 -t major           # Create major release (1.0.0 -> 2.0.0)"
    echo "  $0 -p                 # Create pre-release (1.0.0 -> 1.0.1-alpha.1)"
    echo "  $0 -d                 # Dry run to see what would happen"
    exit 1
}

# Parse command line arguments
while getopts "t:pdsh" opt; do
    case $opt in
        t) RELEASE_TYPE="$OPTARG" ;;
        p) PRERELEASE=true ;;
        d) DRY_RUN=true ;;
        s) SKIP_TESTS=true ;;
        h) usage ;;
        *) usage ;;
    esac
done

echo "🚀 SyLife Automated Release"
echo "=========================="

# Check if we're in a git repository
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    print_error "Not in a git repository"
    exit 1
fi

# Check if working directory is clean
if ! git diff-index --quiet HEAD --; then
    print_error "Working directory is not clean. Please commit or stash changes."
    exit 1
fi

# Make sure we're on the main branch
CURRENT_BRANCH=$(git branch --show-current)
if [[ "$CURRENT_BRANCH" != "master" ]] && [[ "$CURRENT_BRANCH" != "main" ]]; then
    print_warning "Not on main branch (current: $CURRENT_BRANCH)"
    echo "Continue anyway? (y/N)"
    read -r response
    if [[ ! "$response" =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Pull latest changes
print_status "Pulling latest changes..."
git pull origin "$CURRENT_BRANCH"

# Get current version
print_status "Determining version..."

# Try to get version from git tags
CURRENT_VERSION=$(git describe --tags --abbrev=0 2>/dev/null | sed 's/^v//' || echo "0.0.0")

# Parse version components
IFS='.' read -r -a VERSION_PARTS <<< "$CURRENT_VERSION"
MAJOR=${VERSION_PARTS[0]:-0}
MINOR=${VERSION_PARTS[1]:-0}
PATCH=${VERSION_PARTS[2]:-0}

# Calculate new version based on release type
case "$RELEASE_TYPE" in
    "major")
        NEW_MAJOR=$((MAJOR + 1))
        NEW_MINOR=0
        NEW_PATCH=0
        ;;
    "minor")
        NEW_MAJOR=$MAJOR
        NEW_MINOR=$((MINOR + 1))
        NEW_PATCH=0
        ;;
    "patch")
        NEW_MAJOR=$MAJOR
        NEW_MINOR=$MINOR
        NEW_PATCH=$((PATCH + 1))
        ;;
    *)
        print_error "Invalid release type: $RELEASE_TYPE"
        exit 1
        ;;
esac

NEW_VERSION="$NEW_MAJOR.$NEW_MINOR.$NEW_PATCH"

# Add pre-release suffix if needed
if [[ "$PRERELEASE" == true ]]; then
    PRERELEASE_COUNT=$(git tag --list "v$NEW_VERSION-*" | wc -l)
    PRERELEASE_COUNT=$((PRERELEASE_COUNT + 1))
    NEW_VERSION="$NEW_VERSION-alpha.$PRERELEASE_COUNT"
fi

NEW_TAG="v$NEW_VERSION"

print_status "Current version: $CURRENT_VERSION"
print_status "New version: $NEW_VERSION"
print_status "New tag: $NEW_TAG"

if [[ "$DRY_RUN" == true ]]; then
    print_warning "DRY RUN MODE - No changes will be made"
fi

# Run tests unless skipped
if [[ "$SKIP_TESTS" != true ]]; then
    print_status "Running tests..."
    
    # Quick build and test
    if [[ -d "build-dev" ]]; then
        cd build-dev
        if cmake --build . --parallel && ctest --output-on-failure; then
            print_success "Tests passed"
        else
            print_error "Tests failed"
            exit 1
        fi
        cd ..
    else
        # Quick modular test
        if [[ -f "scripts/run_quick_tests.sh" ]]; then
            if ./scripts/run_quick_tests.sh; then
                print_success "Quick tests passed"
            else
                print_error "Quick tests failed"
                exit 1
            fi
        else
            print_warning "No test environment found"
        fi
    fi
fi

# Generate changelog
print_status "Generating changelog..."

CHANGELOG_FILE="CHANGELOG_$NEW_VERSION.md"

# Get commits since last tag
if git tag --list | grep -q .; then
    LAST_TAG=$(git describe --tags --abbrev=0 2>/dev/null || echo "")
    if [[ -n "$LAST_TAG" ]]; then
        COMMITS=$(git log $LAST_TAG..HEAD --pretty=format:"- %s (%h)" --no-merges)
    else
        COMMITS=$(git log --pretty=format:"- %s (%h)" --no-merges)
    fi
else
    COMMITS=$(git log --pretty=format:"- %s (%h)" --no-merges)
fi

# Create detailed changelog
cat > "$CHANGELOG_FILE" << EOF
# SyLife Release $NEW_VERSION

Released: $(date +"%Y-%m-%d")

## What's New

$COMMITS

## Breaking Changes

- Check commit messages for any breaking changes

## Technical Details

- **Build Type**: Release with optimizations
- **Target Platforms**: Linux, Windows, macOS, Web
- **Dependencies**: OpenSiv3D v6.5.0, C++20

## Downloads

Will be available after release:
- **Desktop (Linux x64)**: SyLife-Linux-x64.tar.gz
- **Desktop (Windows x64)**: SyLife-Windows-x64.zip
- **Desktop (macOS)**: SyLife-macOS.dmg
- **Web Version**: SyLife-Web.zip

## Installation

### Desktop
1. Download the appropriate package for your platform
2. Extract/Install the package
3. Run SyLife executable

### Web
1. Download SyLife-Web.zip
2. Extract to web server directory
3. Open index.html in browser

## System Requirements

- **OS**: Linux, Windows 10+, macOS 10.15+
- **RAM**: 4GB minimum, 8GB recommended
- **Graphics**: OpenGL 3.3+ support
- **Storage**: 500MB available space

---

🤖 Generated with [Claude Code](https://claude.ai/code)
EOF

print_success "Changelog generated: $CHANGELOG_FILE"

# Update version in files if they exist
print_status "Updating version in project files..."

# Update CMakeLists.txt if it has version info
if grep -q "project.*VERSION" CMakeLists.txt; then
    if [[ "$DRY_RUN" != true ]]; then
        sed -i "s/project(\([^)]*\)VERSION [0-9.]*/project(\1VERSION $NEW_MAJOR.$NEW_MINOR.$NEW_PATCH/" CMakeLists.txt
        print_success "Updated CMakeLists.txt version"
    else
        print_status "Would update CMakeLists.txt version"
    fi
fi

# Update package.json if it exists
if [[ -f "package.json" ]]; then
    if [[ "$DRY_RUN" != true ]]; then
        sed -i "s/\"version\": \"[^\"]*\"/\"version\": \"$NEW_MAJOR.$NEW_MINOR.$NEW_PATCH\"/" package.json
        print_success "Updated package.json version"
    else
        print_status "Would update package.json version"
    fi
fi

# Show what will be committed
print_status "Changes to be committed:"
git diff --name-only

# Commit version changes if any
if ! git diff-index --quiet HEAD --; then
    if [[ "$DRY_RUN" != true ]]; then
        git add .
        git commit -m "🔖 Bump version to $NEW_VERSION

- Automated version bump for release
- Updated project version files
- Generated changelog

🤖 Generated with [Claude Code](https://claude.ai/code)

Co-Authored-By: Claude <noreply@anthropic.com>"
        print_success "Version changes committed"
    else
        print_status "Would commit version changes"
    fi
fi

# Create and push tag
if [[ "$DRY_RUN" != true ]]; then
    print_status "Creating tag $NEW_TAG..."
    git tag -a "$NEW_TAG" -m "Release $NEW_VERSION

$(head -20 "$CHANGELOG_FILE")

🤖 Generated with [Claude Code](https://claude.ai/code)"
    
    print_status "Pushing tag to origin..."
    git push origin "$NEW_TAG"
    git push origin "$CURRENT_BRANCH"
    
    print_success "Tag $NEW_TAG created and pushed"
else
    print_status "Would create and push tag $NEW_TAG"
fi

# Wait for GitHub Actions to trigger
if [[ "$DRY_RUN" != true ]]; then
    print_status "GitHub Actions will now build and create the release..."
    print_status "Monitor progress at: https://github.com/$(git config --get remote.origin.url | sed 's/.*github.com[:/]\([^/]*\/[^/]*\).*/\1/' | sed 's/\.git$//')/actions"
fi

# Clean up
rm -f "$CHANGELOG_FILE"

echo ""
echo "========================================"
print_success "🎉 Release Process Complete!"
echo "========================================"
echo ""
echo "📋 Release Summary:"
echo "   Version: $NEW_VERSION"
echo "   Tag: $NEW_TAG"
echo "   Type: $RELEASE_TYPE"
if [[ "$PRERELEASE" == true ]]; then
    echo "   Pre-release: Yes"
fi
echo ""
echo "🚀 Next Steps:"
if [[ "$DRY_RUN" != true ]]; then
    echo "1. Monitor GitHub Actions build progress"
    echo "2. Test the release packages when ready"
    echo "3. Update documentation if needed"
    echo "4. Announce the release"
else
    echo "This was a dry run. To create the actual release, run:"
    echo "   $0 $(echo $* | sed 's/-d//')"
fi
echo ""
print_success "Release automation complete! 🚀"