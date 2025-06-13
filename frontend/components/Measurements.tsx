import {
useState
} from 'react';

import MultiSelect from 'react-native-multiple-select';

import {
View,
Button,
} from 'react-native';

import Map from './Map';


type DataView = 'chart' | 'map';


export default function Measurements() {
const [dataView, setDataView] = useState<DataView>('chart');
    const [availableDevices, setAvailableDevices] = useState<string>([
        'dupa',
        'chuj'
    ]);

    const [selectedDevices, setSelectedDevices] = useState<string>([]);

    console.log(selectedDevices)

    const onSelectedItemsChange = (selectedItems: { id: string, name: string }[]) => {
        return setSelectedDevices(selectedItems.map((element) => element.name));
    };

    return (
        <View style={{}}>
            <MultiSelect 
                selectText={'devices'}
                items={availableDevices.map((deviceName) => ({ id: deviceName, name: deviceName }))}
                selectedItems={availableDevices}
                onSelectedItemsChange={onSelectedItemsChange}
            />
            <Button title={dataView} onPress={() => {
                if (dataView === 'chart') {
                    setDataView('map');
                } else {
                    setDataView('chart');
                }
            }}/>
            {
                dataView === 'map' ?
                <Map/> :
                <></>
            }
        </View>
    )
};

