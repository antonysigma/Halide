/* Highlighting 'matched' elements in Viz code */
$('#ir-visualization-tab .matched').each(function () {
    this.onmouseover = function () {
        $('#ir-visualization-tab .matched[id^=' + this.id.split('-')[0] + '-]').addClass('Highlight');
    }
    this.onmouseout = function () {
        $('#ir-visualization-tab .matched[id^=' + this.id.split('-')[0] + '-]').removeClass('Highlight');
    }
});

/* Cross highlighting 'matched' variables (only) */
$('#ir-visualization-tab .matched.variable').each(function () {
    this.onmouseover = function () {
        var name = this.outerText;
        $('.matched.variable').filter((idx, val) => {
            return val.outerText === name;
        }).addClass('Highlight');
    }
    this.onmouseout = function () {
        var name = this.outerText;
        $('.matched.variable').filter((idx, val) => {
            return val.outerText === name;
        }).removeClass('Highlight');
    }
});

/* Expand/Collapse buttons in Viz */
function toggleVizByElem(body) {
    var id = body.id;
    var group = body.parentElement;
    var header = group.firstElementChild;
    const safe_but_slow_code = false;
    if (safe_but_slow_code) {
        var buttonShow = header.querySelector('#' + id + '-show');
        var buttonHide = header.querySelector('#' + id + '-hide');
        var re = /(?:\-([^-]+))?$/;
        var viz_id = re.exec(id)[1];
        var ccost_btn = header.querySelector("#vcc-" + viz_id);
        var dcost_btn = header.querySelector("#vdc-" + viz_id);
        var ccost_tt = header.querySelector("#tooltip-vcc-" + viz_id);
        var dcost_tt = header.querySelector("#tooltip-vdc-" + viz_id);
    } else {
        var buttonHide = header.firstElementChild; // #id-hide
        var buttonShow = header.firstElementChild.nextElementSibling; // #id-show
        var cost_div = header.lastElementChild; // #box-header > .viz-cost-btns
        var ccost_btn = cost_div.firstElementChild; // # vcc-id
        var dcost_btn = cost_div.lastElementChild; // # vdc-id
        var ccost_tt = ccost_btn.firstElementChild; // #tooltip-vcc-id
        var dcost_tt = dcost_btn.firstElementChild; // #tooltip-vdc-id
    }
    if (body.classList.contains("collapsed-viz")) {
        body.classList.remove("collapsed-viz");
        buttonShow.style.display = 'none';
        buttonHide.style.display = 'block';
        if (ccost_btn && dcost_tt) {
            // Update cost indicators
            ccost_color = ccost_btn.getAttribute('line-cost-color');
            dcost_color = dcost_btn.getAttribute('line-cost-color');
            ccost_btn.className = ccost_btn.className.replace(/CostColor\d+/, 'CostColor' + ccost_color);
            dcost_btn.className = dcost_btn.className.replace(/CostColor\d+/, 'CostColor' + dcost_color);
            // Update cost tooltips
            ccost = ccost_btn.getAttribute('line-cost');
            dcost = dcost_btn.getAttribute('line-cost');
            ccost_tt.innerText = 'Op Count: ' + ccost;
            dcost_tt.innerText = 'Bits Moved: ' + dcost;
        }
    } else {
        body.classList.add("collapsed-viz");
        buttonShow.style.display = 'block';
        buttonHide.style.display = 'none';
        if (ccost_btn && dcost_tt) {
            // Update cost indicators
            collapsed_ccost_color = ccost_btn.getAttribute('block-cost-color');
            collapsed_dcost_color = dcost_btn.getAttribute('block-cost-color');
            ccost_btn.className = ccost_btn.className.replace(/CostColor\d+/, 'CostColor' + collapsed_ccost_color);
            dcost_btn.className = dcost_btn.className.replace(/CostColor\d+/, 'CostColor' + collapsed_dcost_color);
            // Update cost tooltips
            collapsed_ccost = ccost_btn.getAttribute('block-cost');
            collapsed_dcost = dcost_btn.getAttribute('block-cost');
            ccost_tt.innerText = 'Op Count: ' + collapsed_ccost;
            dcost_tt.innerText = 'Bits Moved: ' + collapsed_dcost;
        }
    }
};

function toggleViz(id) {
    toggleVizByElem(document.getElementById(id));
}

/* Scroll to visualization from IR code */
function scrollToViz(id) {
    var container = document.getElementById('ir-visualization-tab');
    var scrollToObject = document.getElementById(id);
    makeVizVisible(scrollToObject);
    scrollToObject = scrollToObject.parentElement.parentElement;
    container.scrollTo({
        top: getOffsetTop(scrollToObject) - 8,
        left: getOffsetLeft(scrollToObject) - 8,
        behavior: 'smooth'
    });
    scrollToObject.style.backgroundColor = 'white';
    setTimeout(function () {
        scrollToObject.style.backgroundColor = 'transparent';
    }, 1000);
}

function getOffsetTop(element) {
    if (!element) return 0;
    if (element.id == 'ir-visualization-tab') return 0;
    return getOffsetTop(element.offsetParent) + element.offsetTop;
}

function getOffsetLeft(element) {
    if (!element) return 0;
    if (element.id == 'ir-visualization-tab') return 0;
    return getOffsetLeft(element.offsetParent) + element.offsetLeft;
}

// In case the code we are scrolling to viz block that sits within
// a collapsed parent block, uncollapse it
function makeVizVisible(element) {
    if (!element) return;
    if (element == document) return;
    if (element.classList.contains("collapsed-viz")) {
        toggleViz(element.id);
    }
    makeVizVisible(element.parentNode);
}

/* Resizing visualization tabs */
var codeDiv = document.getElementById('ir-code-tab');
var resizeBar = document.getElementById('resize-bar-1');
var irVizDiv = document.getElementById('ir-visualization-tab');
var resizeBarAssembly = document.getElementById('resize-bar-2');
var assemblyCodeDiv = document.getElementById('assembly-tab');

codeDiv.style.flexGrow = '0';
resizeBar.style.flexGrow = '0';
irVizDiv.style.flexGrow = '0';
resizeBarAssembly.style.flexGrow = '0';
assemblyCodeDiv.style.flexGrow = '0';

codeDiv.style.flexBasis = 'calc(50% - 6px)';
resizeBar.style.flexBasis = '6px';
irVizDiv.style.flexBasis = 'calc(50% - 3px)';
resizeBarAssembly.style.flexBasis = '6px';

var currentResizer;
var mousedown = false;
resizeBar.addEventListener('mousedown', (event) => {
    currentResizer = resizeBar;
    mousedown = true;
});
resizeBarAssembly.addEventListener('mousedown', (event) => {
    currentResizer = resizeBarAssembly;
    mousedown = true;
});
document.addEventListener('mouseup', (event) => {
    currentResizer = null;
    mousedown = false;
});

document.addEventListener('mousemove', (event) => {
    if (mousedown) {
        if (currentResizer == resizeBar) {
            resize(event);
        } else if (currentResizer == resizeBarAssembly) {
            resizeAssembly(event);
        }
    }
});


function resize(e) {
    if (e.x < 25) {
        collapse_code_tab();
        return;
    }

    const size = `${e.x}px`;
    var rect = resizeBarAssembly.getBoundingClientRect();

    if (e.x > rect.left) {
        collapseR_visualization_tab();
        return;
    }

    codeDiv.style.display = 'block';
    irVizDiv.style.display = 'block';
    codeDiv.style.flexBasis = size;
    irVizDiv.style.flexBasis = `calc(${rect.left}px - ${size})`;
}

function resizeAssembly(e) {
    if (e.x > screen.width - 25) {
        collapse_assembly_tab();
        return;
    }

    var rect = resizeBar.getBoundingClientRect();

    if (e.x < rect.right) {
        collapseL_visualization_tab();
        return;
    }

    const size = `${e.x}px`;
    irVizDiv.style.display = 'block';
    assemblyCodeDiv.style.display = 'block';
    irVizDiv.style.flexBasis = `calc(${size} - ${rect.right}px)`;
    assemblyCodeDiv.style.flexBasis = `calc(100% - ${size})`;

}

function collapse_code_tab() {
    irVizDiv.style.display = 'block';
    var rect = resizeBarAssembly.getBoundingClientRect();
    irVizDiv.style.flexBasis = `${rect.left}px`;
    codeDiv.style.display = 'none';
}

function collapseR_visualization_tab() {
    codeDiv.style.display = 'block';
    var rect = resizeBarAssembly.getBoundingClientRect();
    codeDiv.style.flexBasis = `${rect.left}px`;
    irVizDiv.style.display = 'none';
}

function collapseL_visualization_tab() {
    assemblyCodeDiv.style.display = 'block';
    var rect = resizeBar.getBoundingClientRect();
    assemblyCodeDiv.style.flexBasis = `calc(100% - ${rect.right}px)`;
    irVizDiv.style.display = 'none';
}

function collapse_assembly_tab() {
    irVizDiv.style.display = 'block';
    var rect = resizeBar.getBoundingClientRect();
    irVizDiv.style.flexBasis = `calc(100% - ${rect.right}px)`;
    assemblyCodeDiv.style.display = 'none';
}

function depth(elem) {
    if (elem.id == 'ir-visualization-tab') {
      return 0;
    } else if (elem.tagName == 'DIV' && elem.classList.contains("box")) {
        return 1 + depth(elem.parentNode);
    } else {
        return depth(elem.parentNode);
    }
}

// Collapse viz boxes beyond at depth > 1
$('div[id^="viz-"]').filter((idx, val) => {
    return depth(val) > 1;
}).each((idx, val) => {
    toggleVizByElem(val);
});

// Cost model js
var re = /(?:\-([^-]+))?$/;
var cost_btns = $('div[id^="cc-"], div[id^="dc-"]');
for (var i = 0; i < cost_btns.size(); i++) {
    const button = cost_btns[i];
    const highlight_span = document.getElementById("cost-bg-" + re.exec(button.id)[1]); // span#
    $(button).mouseover(() => {
        $(highlight_span).css("background", "#e5e3e3");
    });
    $(button).mouseout(() => {
        $(highlight_span).css("background", "none");
    });
}
