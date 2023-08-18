# Javascript toolchain for IRVisualizer: the single page web application

**Features:**

* To trigger Github's online Dependabot alerts for security vulnerabilities.

* To enable static analyzer for the Javascript/ES6 language.

* To visualize the Halide IR and the assembly code interactively
with a web browser.

**Usage:**

* Execute `make depend` to scan for broken Javascript dependencies.

* Execute `make lint` to scan the main script for potential syntax errors and
  bugs.

**TODO**

* [ ] Rename the extensions from `*.template.html` to `template/*.[js|css|html]`.

* [ ] Modernize the main script to ES6.

* [ ] Import all dependencies with the `import` statement, not via the `<script>` tag.

* [ ] Resolve all static analyzer warnings, e.g. "buttonHide" is already defined, "btns.size()" is undefined, etc.

* [ ] Invoke `make build` to bundle all dependencies, stylesheets, and
  javascripts from the `template/` folder to the output folder
  `distribution/bundle.[js|css]`; to embed all bundled code into the single HTML
  file, so that users can browse the IRVisualizer output without Internet access.