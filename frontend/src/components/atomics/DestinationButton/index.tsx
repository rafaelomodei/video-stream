import { Button } from '@/components/ui/button'; // ShadCN Button Component
import { LucideIcon } from 'lucide-react'; // Tipo de ícone

interface DestinationButtonProps {
  label: string;
  Icon: LucideIcon;
  isSelected: boolean;
  onClick: () => void;
}

const DestinationButton = ({
  label,
  Icon,
  isSelected,
  onClick,
}: DestinationButtonProps) => {
  return (
    <Button
      variant='secondary'
      className={`flex items-center justify-between px-4 p-8 rounded-xl`}
      onClick={onClick}
    >
      <div className='flex items-center space-x-2'>
        <Icon className='w-5 h-5' />
        <span> - - - </span>
      </div>
      <span className='ml-2 font-bold'>{label}</span>
    </Button>
  );
};

export default DestinationButton;