#!/bin/sh
set -e

echo "Creating directory for gh-pages update"
mkdir $BUILD_DIR-docs
cd $BUILD_DIR-docs

git clone -b gh-pages https://git@$GH_REPO_REF
cd $GH_REPO_NAME

echo "Setting up repo"
git config push.default simple
git config user.name "Travis CI"
git config user.email "travis@travis-ci.org"

echo "Cleaning repo"
rm -rf ./*

echo "" > .nojekyll
echo "theme: jekyll-theme-cayman" > _config.yml

if [ -d "$TRAVIS_BUILD_DIR/$BUILD_DIR/UnicornEngine/docs/html" ] && [ -f "$TRAVIS_BUILD_DIR/$BUILD_DIR/UnicornEngine/docs/html/index.html" ]; then
    echo "Copying documentation"
    cp -a "$TRAVIS_BUILD_DIR/$BUILD_DIR/UnicornEngine/docs/html/." ./

    echo "Publishing documentation"
    git add --all
    git commit -m "Documentation deployment to GitHub Pages" -m "Travis build: ${TRAVIS_BUILD_NUMBER}" -m "Commit: ${TRAVIS_COMMIT}"
    git push --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}" > /dev/null 2>&1
else
    echo '' >&2
    echo 'Warning: No documentation (html) files have been found!' >&2
    echo 'Warning: Not going to push the documentation to GitHub!' >&2
    exit 1
fi
