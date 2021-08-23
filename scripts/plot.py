import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


def _data_properties(fname, title, line):
    return {
        'fname': fname,
        'title': title,
        'line': line
    }


def _plot_properties(func, label, subplot):
    return {
        'func': func,
        'label': label,
        'subplot': subplot
    }


def _parse_complex(text):
    # Remove parentheses
    text = text[1:-1]
    real, imag = text.split(",")
    return complex(float(real), float(imag))


def main():
    measurements = _data_properties(sys.argv[1], 'measured', '--')
    simulated = _data_properties(sys.argv[2], 'simulated', '-')
    impedances = {'Z11': 'b', 'Z12': 'r', 'Z22': 'g'}
    legends = [[], []]
    # Plot measurements and then the extracted data
    for data_properties in [measurements, simulated]:
        # Read data
        data = pd.read_table(
            data_properties['fname'], sep=' ', header=0,
            converters={
                'f': float, 'Z11': _parse_complex, 'Z12': _parse_complex, 'Z21': _parse_complex, 'Z22': _parse_complex
            }
        )
        # Plot impedances
        for i, plot_properties in enumerate([_plot_properties(np.real, 'Re', (2, 1, 1)), _plot_properties(np.imag, 'Im', (2, 1, 2))]):
            plt.subplot(*plot_properties['subplot'])
            for j, (impedance, color) in enumerate(impedances.items()):
                func = plot_properties['func']
                plt.semilogx(data['f'] / 1e9, func(data[impedance]), f"{data_properties['line']}{color}", zorder=-j)
                plt.xlabel('f (GHz)')
                plt.ylabel(f"{plot_properties['label']}(Z) (Ω)")
                legends[i].append(f"{impedance} {data_properties['title']}")
    # Format plots
    for i, props in enumerate([measurements, simulated]):
        plt.subplot(2, 1, i + 1)
        plt.xlim([1e-3, 10])
        plt.legend(legends[i], loc='upper right')
        plt.grid(True, which='both')
    plt.margins(0.5)
    plt.show()


if __name__ == '__main__':
    main()
