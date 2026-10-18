import ghidra.app.script.GhidraScript;
import ghidra.framework.options.Options;
import ghidra.program.model.listing.Function;
import java.util.TreeMap;

public class ReportNamed extends GhidraScript {
    @Override
    protected void run() throws Exception {
        Options info = currentProgram.getOptions("Program Information");
        for (String name : info.getOptionNames()) {
            if (name.toLowerCase().contains("psyq")) {
                println("PROGRAM_INFO " + name + " = " + info.getValueAsString(name));
            }
        }

        int total = 0;
        TreeMap<String, Integer> prefixes = new TreeMap<>();
        StringBuilder named = new StringBuilder();
        for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
            total++;
            String n = f.getName();
            if (n.startsWith("FUN_")) continue;
            named.append(f.getEntryPoint()).append(' ').append(n).append('\n');
            String p = n.replaceAll("[^A-Za-z_].*$", "");
            prefixes.merge(p.length() > 4 ? p.substring(0, 4) : p, 1, Integer::sum);
        }
        int count = named.toString().split("\n").length;
        println("TOTAL_FUNCS=" + total + " NAMED_FUNCS=" + count);
        String out = askString("out", "path");
        java.nio.file.Files.writeString(java.nio.file.Path.of(out), named.toString());

        for (String addr : new String[] {"80087f18", "80087bfc", "8002cf28", "8002c808"}) {
            Function f = getFunctionAt(toAddr(addr));
            println("AT " + addr + " -> " + (f == null ? "(no function)" : f.getName()));
        }
    }
}
