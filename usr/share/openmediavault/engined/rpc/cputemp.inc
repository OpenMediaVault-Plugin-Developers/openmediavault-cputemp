<?php
/**
 * Copyright (C) 2022-2024 OpenMediaVault Plugin Developers
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
        $this->registerMethod("get");
        $this->registerMethod("get2");
        $this->registerMethod("get3");
        $this->registerMethod("get4");
        $this->registerMethod("getSensors");
    }

    public function get($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $cpuTemp = \OMV\Environment::get("OMV_CPU_TEMP_COMMAND",
          "cat /sys/devices/virtual/thermal/thermal_zone0/temp");
        $cpuDivisor = \OMV\Environment::get("OMV_CPU_TEMP_DIVISOR", "1000");
        if ($cpuDivisor == 0) {
            $cpuDivisor = 1;
        }
        $cmd = new \OMV\System\Process($cpuTemp);
        $cmd->execute($output, $exitStatus);
        return [
            "cputemp" => intval($output[0]) / $cpuDivisor
        ];
    }

    public function get2($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $cpuTemp = \OMV\Environment::get("OMV_CPU_TEMP_COMMAND2",
          "cat /sys/devices/virtual/thermal/thermal_zone0/temp");
        $cpuDivisor = \OMV\Environment::get("OMV_CPU_TEMP_DIVISOR2", "1000");
        if ($cpuDivisor == 0) {
            $cpuDivisor = 1;
        }
        $cmd = new \OMV\System\Process($cpuTemp);
        $cmd->execute($output, $exitStatus);
        return [
            "cputemp" => intval($output[0]) / $cpuDivisor
        ];
    }

    public function get3($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $cpuTemp = \OMV\Environment::get("OMV_CPU_TEMP_COMMAND3",
          "cat /sys/devices/virtual/thermal/thermal_zone0/temp");
        $cpuDivisor = \OMV\Environment::get("OMV_CPU_TEMP_DIVISOR3", "1000");
        if ($cpuDivisor == 0) {
            $cpuDivisor = 1;
        }
        $cmd = new \OMV\System\Process($cpuTemp);
        $cmd->execute($output, $exitStatus);
        return [
            "cputemp" => intval($output[0]) / $cpuDivisor
        ];
    }

    public function get4($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_EVERYONE]);
        // get cpu temp command and divisor number
        $cpuTemp = \OMV\Environment::get("OMV_CPU_TEMP_COMMAND4",
          "cat /sys/devices/virtual/thermal/thermal_zone0/temp");
        $cpuDivisor = \OMV\Environment::get("OMV_CPU_TEMP_DIVISOR4", "1000");
        if ($cpuDivisor == 0) {
            $cpuDivisor = 1;
        }
        $cmd = new \OMV\System\Process($cpuTemp);
        $cmd->execute($output, $exitStatus);
        return [
            "cputemp" => intval($output[0]) / $cpuDivisor
        ];
    }

    public function getSensors($params, $context)
    {
        // Validate the RPC caller context.
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
