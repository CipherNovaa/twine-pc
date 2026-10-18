// Applies C prototypes (one per line) to existing functions with matching names.
import ghidra.app.cmd.function.ApplyFunctionSignatureCmd;
import ghidra.app.script.GhidraScript;
import ghidra.app.services.DataTypeManagerService;
import ghidra.app.util.cparser.C.CParserUtils;
import ghidra.program.model.data.FunctionDefinitionDataType;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;
import java.nio.file.Files;
import java.nio.file.Path;

public class ApplySignatures extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String path = getScriptArgs().length > 0 ? getScriptArgs()[0] : askString("signatures", "path");
        for (String line : Files.readAllLines(Path.of(path))) {
            line = line.trim();
            if (line.isEmpty() || line.startsWith("#")) continue;
            FunctionDefinitionDataType sig =
                CParserUtils.parseSignature((DataTypeManagerService) null, currentProgram, line);
            if (sig == null) {
                printerr("could not parse: " + line);
                continue;
            }
            for (Function f : getGlobalFunctions(sig.getName())) {
                new ApplyFunctionSignatureCmd(f.getEntryPoint(), sig, SourceType.USER_DEFINED).applyTo(currentProgram);
                println("SIGNATURE " + f.getEntryPoint() + " " + f.getSignature());
            }
        }
    }
}
