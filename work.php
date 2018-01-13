<?php


$util = 'utils/hello';

switch ($_GET["purpose"]) {
case "flow":
	$util = 'utils/flow data/videos/720.mp4';
    break;
case "coarse":
	$util = 'utils/coarse';
    break;
case "refine":
	$util = 'utils/refine';
    break;
}

echo getcwd();


$output = shell_exec($util);
echo "<pre>$output</pre>";
echo exec('whoami');


?>


