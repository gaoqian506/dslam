<?php

switch ($_GET["purpose"]) {
case "flow":
    echo "flow";
    break;
case "coarse":
    echo "coarse";
    break;
case "refine":
    echo "refine";
    break;
}

$output = shell_exec('../utils/hello');
echo "<pre>$output</pre>";
?>
