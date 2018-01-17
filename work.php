<?php


$util = 'bin/hello.bin';

switch ($_GET["purpose"]) {
case "flow":
	$util = 'bin/flow.bin data/videos/720.mp4';
    break;
case "local":
	$util = 'bin/local.bin';
    break;
case "coarse":
	$util = 'bin/coarse.bin';
    break;
case "refine":
	$util = 'bin/refine.bin';
    break;
}

echo getcwd();


$output = shell_exec($util);
echo "<pre>$output</pre>";
echo exec('whoami');


?>


