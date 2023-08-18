import js from "@eslint/js";
import {defineConfig} from "eslint/config";
import globals from "globals";

export default defineConfig(
    [
        js.configs.recommended,
        {
            languageOptions : {
                globals : globals.browser,
                ecmaVersion: 6,
            },
            rules : {
                "comma-dangle" : [ "error", "only-multiline" ],
            },
        },
    ]);
