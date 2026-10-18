// Applies a splat-format symbol_addrs file: `name = 0xADDR; // type:func|data`
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ApplySymbols extends GhidraScript {
    private static final Pattern LINE =
        Pattern.compile("^\\s*([A-Za-z_]\\w*)\\s*=\\s*(0x[0-9A-Fa-f]+)\\s*;\\s*(?://(.*))?$");

    @Override
    protected void run() throws Exception {
        String path = getScriptArgs().length > 0 ? getScriptArgs()[0] : askString("symbols", "path");
        int applied = 0;
        for (String raw : Files.readAllLines(Path.of(path))) {
            Matcher m = LINE.matcher(raw);
            if (!m.matches()) continue;
            String name = m.group(1);
            Address addr = toAddr(Long.parseLong(m.group(2).substring(2), 16));
            String attrs = m.group(3) == null ? "" : m.group(3);

            if (attrs.contains("type:func")) {
                Function f = getFunctionAt(addr);
                if (f == null) {
                    disassemble(addr);
                    f = createFunction(addr, name);
                }
                if (f == null) {
                    printerr("could not create function " + name + " @ " + addr);
                    continue;
                }
                f.setName(name, SourceType.USER_DEFINED);
            } else {
                createLabel(addr, name, true, SourceType.USER_DEFINED);
            }
            println("APPLIED " + name + " @ " + addr);
            applied++;
        }
        println("APPLIED_TOTAL=" + applied);
    }
}
