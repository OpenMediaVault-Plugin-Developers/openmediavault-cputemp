<?php
/**
 * Copyright (C) 2022-2026 openmediavault plugin developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
class OMVRpcServiceCpuTemp extends \OMV\Rpc\ServiceAbstract
{
    public function getName()
    {
        return "CpuTemp";
    }

    public function initialize()
    {
        $this->registerMethod("getSettings");
        $this->registerMethod("setSettings");
        $this->registerMethod("get");
        $this->registerMethod("get2");
        $this->registerMethod("get3");
        $this->registerMethod("get4");
        $this->registerMethod("getSensors");
    }

    public function getSettings($params, $context)
    {
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        $db = \OMV\Config\Database::getInstance();
        $result = $db->getAssoc("conf.service.cputemp");
        $result['currenttemp1'] = $this->callScriptSafe($result['scriptpath'], $result['divisor']);
        $result['currenttemp2'] = empty($result['script2']) ? "" : $this->callScriptSafe($result['scriptpath2'], $result['divisor2']);
        $result['currenttemp3'] = empty($result['script3']) ? "" : $this->callScriptSafe($result['scriptpath3'], $result['divisor3']);
        $result['currenttemp4'] = empty($result['script4']) ? "" : $this->callScriptSafe($result['scriptpath4'], $result['divisor4']);
        return $result;
    }

    public function setSettings($params, $context)
    {
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        $this->validateMethodParams($params, "rpc.cputemp.setSettings");
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get("conf.service.cputemp");
        $object->setAssoc($params);
        $db->set($object);
        return $object->getAssoc();
    }

    private function callScript($scriptPath, $divisor)
    {
        if ($divisor <= 0) {
            $divisor = 1;
        }
        $cmd = new \OMV\System\Process($scriptPath);
        $cmd->execute($output, $exitStatus);
        return intval($output[0]) / $divisor;
    }

    private function callScriptSafe($scriptPath, $divisor)
    {
        try {
            return number_format($this->callScript($scriptPath, $divisor), 1) . " °C";
        } catch (\Exception $e) {
            return "Error: " . $e->getMessage();
        }
    }

    public function get($params, $context)
    {
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get("conf.service.cputemp");
        return [
            "cputemp" => $this->callScript(
                $object->get("scriptpath"),
                $object->get("divisor")
            )
        ];
    }

    public function get2($params, $context)
    {
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get("conf.service.cputemp");
        if (empty($object->get("script2"))) {
            return ["cputemp" => 0];
        }
        return [
            "cputemp" => $this->callScript(
                $object->get("scriptpath2"),
                $object->get("divisor2")
            )
        ];
    }

    public function get3($params, $context)
    {
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get("conf.service.cputemp");
        if (empty($object->get("script3"))) {
            return ["cputemp" => 0];
        }
        return [
            "cputemp" => $this->callScript(
                $object->get("scriptpath3"),
                $object->get("divisor3")
            )
        ];
    }

    public function get4($params, $context)
    {
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get("conf.service.cputemp");
        if (empty($object->get("script4"))) {
            return ["cputemp" => 0];
        }
        return [
            "cputemp" => $this->callScript(
                $object->get("scriptpath4"),
                $object->get("divisor4")
            )
        ];
    }

    public function getSensors($params, $context)
    {
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        $output = [];
        $temps = [];
        if (file_exists("/usr/bin/sensors")) {
            $cmd = new \OMV\System\Process("sensors","| cut -d'(' -f1");
            $cmd->execute($output, $exitStatus);

            foreach (array_filter($output) as $line) {
                if (strpos($line, ':') > 0) {
                    $temps[] = trim($line);
                } else {
                    $temps[] = '<br/><b>'.trim($line).'</b><br/>';
                }
            }
        } else {
            $temps[] = "lm-sensors package is not installed and/or configured.";
        }
        return (implode('<br/>', $temps));
    }
}
