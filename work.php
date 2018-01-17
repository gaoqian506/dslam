<?php


$util = 'utils/hello.utl';

switch ($_GET["purpose"]) {
case "flow":
	$util = 'utils/flow.utl data/videos/720.mp4';
    break;
case "local":
	$util = 'utils/local.utl';
    break;
case "coarse":
	$util = 'utils/coarse.utl';
    break;
case "refine":
	$util = 'utils/refine.utl';
    break;
}

echo getcwd();


$output = shell_exec($util);
echo "<pre>$output</pre>";
echo exec('whoami');


?>


